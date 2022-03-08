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
			   TIM_HandleTypeDef* debug_clk)
{
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
	}

	for (int i = 0; i < SNOWBALL_COUNT; i++)
	{
		snowball[i].setId(SNOWBALL_ID + i);
		snowball[i].setW(SNOWBALL_W);
		snowball[i].setH(SNOWBALL_H);
	}

	for (int i = 0; i < POWERUP_COUNT; i++)
		powerup[i].setId(POWERUP_ID + i);

	for (int i = 0; i < ICECUBE_COUNT; i++)
		icecube[i].setId(ICECUBE_ID + i);

	///////////////////////////////////

	player[0].setX(400);
	player[0].setY(400);
	player[0].setEnabled(1);

	snowball[1].setY_spd(80);
	snowball[1].setX_spd(-30);
	snowball[1].setY(200);
	snowball[1].setX(300);
	snowball[1].setEnabled(1);

	snowball[2].setY_spd(-20);
	snowball[2].setX_spd(-40);
	snowball[2].setY(150);
	snowball[2].setX(500);
	snowball[2].setEnabled(1);

	snowball[3].setX_spd(125);
	snowball[3].setY(350);
	snowball[3].setX(200);
	snowball[3].setEnabled(1);
//
	snowball[4].setY_spd(-40);
	snowball[4].setX_spd(100);
	snowball[4].setY(300);
	snowball[4].setX(300);
	snowball[4].setEnabled(1);
}

Engine::~Engine() {
	// TODO Auto-generated destructor stub
}

/////////////////// CORE METHODS ///////////////////////
////////////////////////////////////////////////////////
void Engine::gameLoop()
{
	static uint8_t r = 0;
	static uint16_t t = 0;

//	gameUpdate();

	uint16_t clk_t = getTime(refresh_clk);

	if ( clk_t >= REFRESH_PRESCALER)
	{
		gameUpdate();
		t += clk_t;
		r++;

		if (r >= 40)
		{
			char str1[20];
			sprintf( str1, "%d", getTime(debug_clk) );
			debugPrintln( str1 );

			setTime(debug_clk, 0);
			r = 0;
		}

		screenUpdate();
		setTime(refresh_clk, 0);
	}
}


void Engine::gameUpdate()
{
	playerInput();

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
}


void Engine::screenUpdate()
{
	for (int i = 0; i < PLAYER_COUNT; i++)
		if (player[i].hasChanged())
			spi.send_gfx_packet(player[i]);

	for (int i = 0; i < SNOWBALL_COUNT; i++)
		if (snowball[i].hasChanged())
			spi.send_gfx_packet(snowball[i]);

	spi.send_confirm();
}

/////////////////// SPRITE METHODS ///////////////////////
//////////////////////////////////////////////////////////

void Engine::spritePosUpdate(IObject* spr)
{
	spriteSubpixUpdate(spr);

	uint8_t coll = isColliding(spr);

	if ( coll & (COLL_RIGHT | COLL_LEFT) && spr->hasChanged(CHECK_X) )
		spr->x_spd *= -1;

	if ( coll & (COLL_BOTTOM | COLL_TOP) && spr->hasChanged(CHECK_Y) )
		spr->y_spd *= -1;
}

void Engine::spriteSubpixUpdate(IObject* s)
{
	s->x_sub += s->x_spd;
	s->y_sub += s->y_spd;

	uint8_t coll = isColliding(s);

	if (s->x_sub > SUBPIX_MAX)
	{
		if ( not ( coll & (COLL_RIGHT) ) )
			s->x++;

		s->x_sub %= SUBPIX_MAX;
	}
	else if (s->x_sub < 0)
	{
		if ( not ( coll & (COLL_LEFT) ) )
			s->x--;

		s->x_sub = SUBPIX_MAX;
	}

	if (s->y_sub > SUBPIX_MAX)
	{
		if ( not ( coll & (COLL_BOTTOM) ) )
			s->y++;

		s->y_sub %= SUBPIX_MAX;
	}
	else if (s->y_sub < 0)
	{
		if ( not ( coll & (COLL_TOP) ) )
			s->y--;

		s->y_sub = SUBPIX_MAX;
	}
}

uint8_t Engine::isColliding(IObject* spr)
{
	uint8_t res = 0;

	uint16_t xmax = spr->getX() + (spr->getW()) / 2;
	uint16_t xmin = spr->getX() - (spr->getW()) / 2;
	uint16_t ymax = spr->getY() + (spr->getH()) / 2;
	uint16_t ymin = spr->getY() - (spr->getH()) / 2;

	if (xmax >= XMAX) res |= 1 << 0; // Collision on right side
	if (xmin <= XMIN) res |= 1 << 1; // Collision on left side
	if (ymax >= YMAX) res |= 1 << 2; // Collision on bottom side
	if (ymin <= YMIN) res |= 1 << 3; // Collision on top side

	return res;
}

/////////////////// MISC METHODS ///////////////////////
////////////////////////////////////////////////////////

void Engine::playerInput()
{
    if (getInput(P1_UP_PORT, P1_UP_PIN))
    {
        player[0].setY_spd(-75);
    }
    else if (getInput(P1_DOWN_PORT, P1_DOWN_PIN))
    {
        player[0].setY_spd(75);
    }
    else if (getInput(P1_LEFT_PORT, P1_LEFT_PIN))
    {
        player[0].setX_spd(-75);
    }
    else if (getInput(P1_RIGHT_PORT, P1_RIGHT_PIN))
        player[0].setX_spd(75);
    else
    {
        player[0].setX_spd(0);
        player[0].setY_spd(0);
    }
}

//////////////////////////

/////////////////// HELPER METHODS ///////////////////////
//////////////////////////////////////////////////////////

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
