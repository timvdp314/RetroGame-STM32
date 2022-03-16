/*
* Engine.cpp
*
*  Created on: 20 Feb 2022
*      Author: timvd
*/

#include <Engine/Engine.h>
#include "../debug.c"

Engine::Engine() {
	// TODO Auto-generated constructor stub
}

Engine::Engine(TIM_HandleTypeDef* refresh_clk,
			   TIM_HandleTypeDef* tick_clk,
			   TIM_HandleTypeDef* debug_clk,
			   uint32_t rand_seed)
{
	this->rand_seed = rand_seed;

	//Screen refresh clock
	this->refresh_clk = refresh_clk;
	this->tick_clk = tick_clk;
	this->debug_clk = debug_clk;

	/// Entities initialisation ///

	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		player[i].setId(PLAYER_ID + i);
		player[i].setW(PLAYER_W);
		player[i].setH(PLAYER_H);
		player[i].setType(SPR::PLAYER);
	}

	for (int i = 0; i < SNOWBALL_COUNT; i++)
	{
		snowball[i].setId(SNOWBALL_ID + i);
		snowball[i].setW(SNOWBALL_W);
		snowball[i].setH(SNOWBALL_H);
		snowball[i].setType(SPR::SNOWBALL);
	}

	for (int i = 0; i < POWERUP_COUNT; i++)
	{
		powerup[i].setId(POWERUP_ID + i);
		powerup[i].setType(SPR::PWUP);
	}

	for (int i = 0; i < ICECUBE_COUNT; i++)
	{
		icecube[i].setId(ICECUBE_ID + i);
		icecube[i].setType(SPR::ICECUBE);
	}

	///////////////////////////////////

	player[0].setX( (XMAX - XMIN) / 2 + XMIN );
	player[0].setY( (YMAX - YMIN) / 2 + YMIN );
	player[0].setEnabled(1);
	player[0].setId(2);

//	player[1].setX(400);
//	player[1].setY(400);
//	player[1].setX_spd(0);
//	player[1].setY_spd(0);
//	player[1].setEnabled(0);

	snowball[0].setEnabled(1);
	snowball[1].setEnabled(1);
	snowball[2].setEnabled(1);
	snowball[3].setEnabled(1);
	snowball[4].setEnabled(1);
//
//	icecube[0].setId(8);
//	icecube[0].setY(175);
//	icecube[0].setX(200);
//	icecube[0].setEnabled(1);
//
//	icecube[1].setId(9);
//	icecube[1].setY(300);
//	icecube[1].setX(400);
//	icecube[1].setEnabled(1);
}

Engine::~Engine() {
	// TODO Auto-generated destructor stub
}

/// -- Core methods -- ///
//////////////////////////

void Engine::gameLoop()
{
	uint16_t clk_t = getTime(refresh_clk);

	if ( clk_t >= REFRESH_PRESCALER)
	{
		screenUpdate();
		gameUpdate();
		checkRefreshRate();

		setTime(refresh_clk, 0);
	}
}


void Engine::gameUpdate()
{
	playerInput();
	snowballSpawner();

	if ( getTime(tick_clk) )
	{
		spritesUpdate();
		setTime(tick_clk, 0);
	}
}


void Engine::spritesUpdate()
{
	for (int i = 0; i < PLAYER_COUNT; i++)
		if ( player[i].getEnabled() && (player[i].getX_spd() || player[i].getY_spd()) )
			spritePosUpdate(&player[i]);

	for (int i = 0; i < SNOWBALL_COUNT; i++)
		if ( snowball[i].getEnabled() && (snowball[i].getX_spd() || snowball[i].getY_spd()) )
			spritePosUpdate(&snowball[i]);

	for (int i = 0; i < ICECUBE_COUNT; i++)
		if ( icecube[i].getEnabled() && (icecube[i].getX_spd() || icecube[i].getY_spd()) )
			spritePosUpdate(&icecube[i]);

	isColliding(&player[0]);
}


void Engine::screenUpdate()
{
	for (int i = 0; i < PLAYER_COUNT; i++)
		if (player[i].hasChanged())
			spi.send_gfx_packet(player[i]);

	for (int i = 0; i < SNOWBALL_COUNT; i++)
		if (snowball[i].hasChanged())
			spi.send_gfx_packet(snowball[i]);

	for (int i = 0; i < ICECUBE_COUNT; i++)
		if (icecube[i].hasChanged())
			spi.send_gfx_packet(icecube[i]);

	spi.send_confirm();
}

/// -- Sprite methods -- ///
////////////////////////////

void Engine::spritePosUpdate(IObject* spr)
{
	spriteSubpixUpdate(spr);
//	printSpriteInfo(spr);

//
//	if ( coll & (COLL_RIGHT | COLL_LEFT) && spr->hasChanged(CHECK_X) )
//		spr->x_spd *= -1;
//
//	if ( coll & (COLL_BOTTOM | COLL_TOP) && spr->hasChanged(CHECK_Y) )
//		spr->y_spd *= -1;
}

void Engine::spriteSubpixUpdate(IObject* s)
{
	uint16_t* x = &s->x; uint16_t* y = &s->y;
	int16_t* x_sub = &s->x_sub; int16_t* y_sub = &s->y_sub;
	int16_t* x_spd = &s->x_spd; int16_t* y_spd = &s->y_spd;

	*x_sub += *x_spd * (TICK_FREQ / REFRESH_RATE);
	*y_sub += *y_spd * (TICK_FREQ / REFRESH_RATE);

	uint8_t coll = isColliding(s);

	if (*x_sub > SUBPIX_MAX)
	{
		*x += (*x_sub / SUBPIX_MAX);

		*x_sub %= SUBPIX_MAX;
	}
	else if (*x_sub < 0)
	{
		*x += (*x_sub / SUBPIX_MAX) - 1;

		*x_sub = SUBPIX_MAX + (*x_sub % SUBPIX_MAX);
	}

	if (*y_sub > SUBPIX_MAX)
	{
		*y+= (*y_sub / SUBPIX_MAX);

		*y_sub %= SUBPIX_MAX;
	}
	else if (*y_sub < 0)
	{
		*y += (*y_sub / SUBPIX_MAX) - 1;

		*y_sub = SUBPIX_MAX + (*y_sub % SUBPIX_MAX);
	}
}

uint8_t Engine::isColliding(IObject* s)
{
	uint8_t res = 0;

	uint16_t xmin = s->x - s->w / 2;
	uint16_t xmax = s->x + s->w / 2;
	uint16_t ymin = s->y - s->h / 2;
	uint16_t ymax = s->y + s->h / 2;

	IObject* o;

	if (s->type == SPR::PLAYER)
	{
		for (int i = 0; i < SNOWBALL_COUNT; i++)
		{
			o = &snowball[i];

			if (snowball[i].getEnabled())
			{
				for (int i = 0; i < 4; i++)
				{
					if (o->corner[i].x <= xmax && o->corner[i].x >= xmin &&
						o->corner[i].y <= ymax && o->corner[i].y >= ymin)
					{
						for (int j = 0; j < SNOWBALL_COUNT; j++)
							snowball[j].x = XMIN - 20;

						s->x = (XMAX - XMIN) / 2 + XMIN;
						s->y = (YMAX - YMIN) / 2 + YMIN;
					}
				}
			}
		}
	}

//	if (xmax >= XMAX) res |= 1 << 0; // Collision on right side
//	if (xmin <= XMIN) res |= 1 << 1; // Collision on left side
//	if (ymax >= YMAX) res |= 1 << 2; // Collision on bottom side
//	if (ymin <= YMIN) res |= 1 << 3; // Collision on top side

	return res;
}

/// -- Misc methods -- ///
//////////////////////////

void Engine::playerInput()
{
    if (getInput(P1_UP_PORT, P1_UP_PIN))
        player[0].setY_spd(-PLAYER_SPD);
    else if (getInput(P1_DOWN_PORT, P1_DOWN_PIN))
        player[0].setY_spd(PLAYER_SPD);
    else
    	player[0].setY_spd(0);

    if (getInput(P1_LEFT_PORT, P1_LEFT_PIN))
        player[0].setX_spd(-PLAYER_SPD);
    else if (getInput(P1_RIGHT_PORT, P1_RIGHT_PIN))
        player[0].setX_spd(PLAYER_SPD);
    else
        player[0].setX_spd(0);
}

void Engine::snowballSpawner()
{
	for (int i = 0; i < SNOWBALL_COUNT; i++)
	{
		if ( snowball[i].getEnabled() )
		{
			if ( snowball[i].getX() < XMIN ||
				 snowball[i].getX() > XMAX ||
				 snowball[i].getY() < YMIN ||
				 snowball[i].getY() > YMAX )
			{
				snowballRandomise(&snowball[i]);
			}
		}
	}
}

void Engine::snowballRandomise(IObject* s)
{
	srand( rand() % rand_seed );
	rand_seed = rand();

	uint8_t side = rand() % 4;

	switch (side)
	{
	case 0:
		s->setX( rand() % (XMAX - XMIN) + XMIN);
		s->setY( YMIN + 1 );
		s->setX_spd(0);
		s->setY_spd( rand() % 100 + MIN_SPD);
		break;

	case 1:
		s->setX( XMAX - 1 );
		s->setY( rand() % (YMAX - YMIN) + YMIN);
		s->setX_spd( -(rand() % 100) - MIN_SPD);
		s->setY_spd( 0 );
		break;

	case 2:
		s->setX( rand() % (XMAX - XMIN) + XMIN);
		s->setY( YMAX - 1 );
		s->setX_spd(0);
		s->setY_spd( -(rand() % 100) - MIN_SPD);
		break;

	case 3:
		s->setX( XMIN + 1 );
		s->setY( rand() % (YMAX - YMIN) + YMIN);
		s->setX_spd( rand() % 100 + MIN_SPD);
		s->setY_spd( 0 );
		break;
	}
}

/// -- Helper methods -- ///
///////////////////////////

uint16_t Engine::getTime(TIM_HandleTypeDef* clk)
{
	return __HAL_TIM_GET_COUNTER(clk);
}

void Engine::setTime(TIM_HandleTypeDef* clk, uint16_t time)
{
	__HAL_TIM_SET_COUNTER(clk, time);
}

bool Engine::getInput(GPIO_TypeDef* port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, pin);
}

/// -- Debug methods -- ///
///////////////////////////

void Engine::checkRefreshRate()
{
	frame++;

	if (frame >= REFRESH_RATE)
	{
		char str1[20];
		sprintf( str1, "%d", getTime(debug_clk) );
		debugPrintln( str1 );

		setTime(debug_clk, 0);
		frame = 0;
	}
}

void Engine::printSpriteInfo(IObject* spr)
{
	 char str1[25];
	 sprintf(str1, "X: %d, Y: %d", spr->x, spr->y);
	 debugPrintln( str1 );
	 sprintf(str1, "X-speed: %d, Y-speed: %d", spr->x_spd, spr->y_spd);
	 debugPrintln( str1 );
	 sprintf(str1, "X-sub: %d, Y-sub: %d", spr->x_sub, spr->y_sub);
	 debugPrintln( str1 );
	 debugPrintln("---");
}
