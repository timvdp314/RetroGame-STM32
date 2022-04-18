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

	for (int i = 0; i < ICECUBE_COUNT; i++)
	{
		icecube[i].setId(ICECUBE_ID + i);
		icecube[i].setType(SPR::ICECUBE);
	}

	for (int i = 0; i < GRAPHIC_COUNT; i++)
	{
		graphic[i].setId(GRAPHIC_ID + i);
		graphic[i].setType(SPR::GRAPHIC);
	}

	///////////////////////////////////

	player[0].setEnabled(1);
	player[0].setId(4);

	player[1].setEnabled(1);
	player[1].setId(8);
	player[1].walk_anim = 6000;

	snowball[0].setEnabled(1);
	snowball[1].setEnabled(1);
	snowball[2].setEnabled(1);
//	snowball[3].setEnabled(1);
//	snowball[4].setEnabled(1);
//

	icecube[0].setId(14);
	icecube[0].setY(175);
	icecube[0].setX(200);
	icecube[0].setW(64);
	icecube[0].setH(80);
	icecube[0].setEnabled(1);

	icecube[1].setId(15);
	icecube[1].setY(400);
	icecube[1].setX(400);
	icecube[1].setW(160);
	icecube[1].setH(32);
	icecube[1].setEnabled(1);

	icecube[2].setY(300);
	icecube[2].setX(650);
	icecube[2].setW(128);
	icecube[2].setH(64);
	icecube[2].setEnabled(1);

	graphic[0].setEnabled(1);
	graphic[1].setEnabled(1);
	graphic[2].setEnabled(1);
	graphic[3].setEnabled(1);
	graphic[4].setEnabled(1);
	graphic[5].setEnabled(1);

	screenClear();

	gameInit();
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

		if (state == GAME_STATE::MENU)
		{
			menuLoop();

			if (game_init)
				gameInit();
		}
		else if (state == GAME_STATE::PLAY)
		{
			gameUpdate();
			checkRefreshRate();
		}

		setTime(refresh_clk, 0);
	}
}

void Engine::menuLoop()
{
	if (!getInput(P_IN[0][P_ACTION]) && !getInput(P_IN[1][P_ACTION]))
	{
		for (int i = 0; i < 2; i++)
		{
			player[i].walk_anim += getTime(tick_clk);

			 char str1[20];
			 sprintf(str1, "%d", player[i].walk_anim);
			 debugPrintln( str1 );

			if (player[i].walk_anim >= 0 && player[i].walk_anim < 2000)
			{
				player[i].x_spd = PLAYER_WALK_SPD;
				player[i].y_spd = 0;
			}
			else if (player[i].walk_anim >= 2000 && player[i].walk_anim < 4000)
			{
				player[i].x_spd = 0;
				player[i].y_spd = PLAYER_WALK_SPD;
			}
			else if (player[i].walk_anim >= 4000 && player[i].walk_anim < 6000)
			{
				player[i].x_spd = -PLAYER_WALK_SPD;
				player[i].y_spd = 0;
			}
			else if (player[i].walk_anim >= 6000 && player[i].walk_anim < 8000)
			{
				player[i].x_spd = 0;
				player[i].y_spd = -PLAYER_WALK_SPD;
			}

			player[i].walk_anim %= 8000;
		}

		spritesUpdate();
		setTime(tick_clk, 0);

		return;
	}

	state = GAME_STATE::PLAY;
	game_init = 1;
}

void Engine::gameInit()
{
	player[0].x = PLAYER1_STARTX;
	player[0].y = PLAYER1_STARTY;

	player[1].x = PLAYER2_STARTX;
	player[1].y = PLAYER2_STARTY;

	for (int i = 0; i < SNOWBALL_COUNT; i++)
		snowball[i].x = XMIN - 20;

	icecube[0].setY(175);
	icecube[0].setX(200);

	icecube[1].setY(400);
	icecube[1].setX(400);

	icecube[2].setY(300);
	icecube[2].setX(650);

	for (int i = 0; i < 6; i++)
	{
		if (i < 3)
			graphic[i].x = XMIN + LIFE_XDIST + i * LIFE_W * 2;
		else
			graphic[i].x = XMAX - LIFE_XDIST - (i - 3) * LIFE_W * 2;

		graphic[i].y = YMIN + LIFE_YDIST;
	}

	updatePlayer(&player[0]);
	updatePlayer(&player[1]);

	player[0].hp = 3;
	player[1].hp = 3;

	game_init = 0;
}

void Engine::gameUpdate()
{
	playerInput(0);
	playerInput(1);

	snowballSpawner();
	despawnIcecubes();

	spritesUpdate();

	setTime(tick_clk, 0);
}


void Engine::spritesUpdate()
{
	for (int i = 0; i < PLAYER_COUNT; i++)
		if ( player[i].getEnabled() && (player[i].getX_spd() || player[i].getY_spd()) )
			spritePosUpdate(&player[i]);

	for (int i = 0; i < SNOWBALL_COUNT; i++)
		if ( snowball[i].getEnabled() && (snowball[i].getX_spd() || snowball[i].getY_spd()) && snowball[i].spawned)
			spritePosUpdate(&snowball[i]);

	for (int i = 0; i < ICECUBE_COUNT; i++)
		if ( icecube[i].getEnabled() && (icecube[i].getX_spd() || icecube[i].getY_spd()) )
			spritePosUpdate(&icecube[i]);
}


void Engine::screenUpdate()
{
	for (int i = 0; i < PLAYER_COUNT; i++)
		if (player[i].hasChanged() || game_init)
			spi.send_gfx_packet(player[i]);

	for (int i = 0; i < SNOWBALL_COUNT; i++)
		if (snowball[i].hasChanged() || game_init)
			spi.send_gfx_packet(snowball[i]);

	for (int i = 0; i < ICECUBE_COUNT; i++)
	{
		icecube[i].x_spd = 0;
		icecube[i].y_spd = 0;

		if (icecube[i].hasChanged() || game_init)
			spi.send_gfx_packet(icecube[i]);
	}

	for (int i = 0; i < GRAPHIC_COUNT; i++)
		if (graphic[i].hasChanged() || game_init)
			spi.send_gfx_packet(graphic[i]);

	spi.send_confirm();
}

void Engine::screenClear()
{
	for (int i = 0; i < PLAYER_COUNT; i++)
		spi.send_gfx_packet(player[i].en, player[i].id, 0, 0);

	for (int i = 0; i < SNOWBALL_COUNT; i++)
		spi.send_gfx_packet(snowball[i].en, snowball[i].id, 0, 0);

	for (int i = 0; i < ICECUBE_COUNT; i++)
		spi.send_gfx_packet(icecube[i].en, icecube[i].id, 0, 0);
}

void Engine::gameOver()
{
	game_init = 1;
	state = GAME_STATE::MENU;
}

/// -- Sprite methods -- ///
////////////////////////////

void Engine::spritePosUpdate(IObject* spr)
{
	if (spr == &player[0] || spr == &player[1])
		updatePlayer(spr);

	checkCollision(spr);
	spriteSubpixUpdate(spr);
}

void Engine::spriteSubpixUpdate(IObject* s)
{
	uint16_t* x = &s->x; uint16_t* y = &s->y;
	int16_t* x_sub = &s->x_sub; int16_t* y_sub = &s->y_sub;
	int16_t* x_spd = &s->x_spd; int16_t* y_spd = &s->y_spd;

	*x_sub += *x_spd * (TICK_FREQ / REFRESH_RATE);
	*y_sub += *y_spd * (TICK_FREQ / REFRESH_RATE);

	if (*x_sub > SUBPIX_MAX)
	{
		*x += (*x_sub / SUBPIX_MAX) * s->isFree[FREE_RIGHT];

		*x_sub %= SUBPIX_MAX;
	}
	else if (*x_sub < 0)
	{
		*x += ( (*x_sub / SUBPIX_MAX) - 1 ) * s->isFree[FREE_LEFT];

		*x_sub = SUBPIX_MAX + (*x_sub % SUBPIX_MAX);
	}

	if (*y_sub > SUBPIX_MAX)
	{
		*y+= (*y_sub / SUBPIX_MAX) * s->isFree[FREE_BOTTOM];

		*y_sub %= SUBPIX_MAX;
	}
	else if (*y_sub < 0)
	{
		*y += ( (*y_sub / SUBPIX_MAX) - 1 ) * s->isFree[FREE_TOP];

		*y_sub = SUBPIX_MAX + (*y_sub % SUBPIX_MAX);
	}
}

void Engine::checkCollision(IObject* s)
{
	uint8_t res = 0;

	IObject* o;
	Player* p;

	uint16_t xmin_s = s->x - s->w / 2;
	uint16_t xmax_s = s->x + s->w / 2;
	uint16_t ymin_s = s->y - s->h / 2;
	uint16_t ymax_s = s->y + s->h / 2;

	if (s->type == SPR::SNOWBALL)
	{
		// Player -> snowball collision
		for (int i = 0; i < PLAYER_COUNT; i++)
		{
			p = &player[i];

			if (p->en)
			{
				if ( checkAnyCollision(s, p) )
				{
					if ( p->hp > 1 )
					{
						p->hp--;
						s->x = XMIN - 15;
						checkHealth(p);
					}
					else
					{
						p->hp = 0;
						checkHealth(p);

						gameOver();
					}
				}
			}
		}

		/// Check ice cubes
		for (int i = 0; i < ICECUBE_COUNT; i++)
		{
			o = &icecube[i];

			if (o->en)
				if ( checkAnyCollision(s, o) )
					s->x = XMIN - 20;
		}
	}

	if (s->type == SPR::PLAYER)
	{
		/// Reset collision check
		for (int i = 0; i < 4; i++) s->isFree[i] = 1;

		/// Check screen borders
		if (xmin_s <= XMIN) s->isFree[FREE_LEFT] = 0;
		if (xmax_s >= XMAX) s->isFree[FREE_RIGHT] = 0;
		if (ymin_s <= YMIN) s->isFree[FREE_TOP] = 0;
		if (ymax_s >= YMAX) s->isFree[FREE_BOTTOM] = 0;

		/// Check ice cubes
		for (int i = 0; i < ICECUBE_COUNT; i++)
		{
			o = &icecube[i];

			if (o->en)
			{
				res = checkSidesCollision(s, o);

				for (int i = 0; i < 4; i++)
					if ( (res >> i) & 1 )
					{
//						if ( (s->x_spd > 0 && i == FREE_RIGHT) || (s->x_spd < 0 && i == FREE_LEFT) )
//							o->x_spd = s->x_spd*2;
//
//						if ( (s->y_spd < 0 && i == FREE_TOP) || (s->y_spd > 0 && i == FREE_BOTTOM) )
//							o->y_spd = s->y_spd*2;

						s->isFree[i] = 0;
					}
			}
		}

		/// Check other player
		if (s == &player[0])
			o = &player[1];
		else
			o = &player[0];

		if (o->en)
		{
			res = checkSidesCollision(s, o);

			for (int i = 0; i < 4; i++)
				if ( (res >> i) & 1 )
					s->isFree[i] = 0;
		}
	}
}

bool Engine::checkAnyCollision(IObject* s, IObject* o)
{
	uint16_t xmin_s = s->x - s->w / 2;
	uint16_t xmax_s = s->x + s->w / 2;
	uint16_t ymin_s = s->y - s->h / 2;
	uint16_t ymax_s = s->y + s->h / 2;

	uint16_t xmin_o = o->x - o->w / 2;
	uint16_t xmax_o = o->x + o->w / 2;
	uint16_t ymin_o = o->y - o->h / 2;
	uint16_t ymax_o = o->y + o->h / 2;

	return ( not (xmin_s >= xmax_o || xmax_s <= xmin_o ||
			 ymin_s >= ymax_o || ymax_s <= ymin_o) );
}

uint8_t Engine::checkSidesCollision(IObject* s, IObject* o)
{
	uint8_t res = 0;

	uint16_t xmin_s = s->x - s->w / 2;
	uint16_t xmax_s = s->x + s->w / 2;
	uint16_t ymin_s = s->y - s->h / 2;
	uint16_t ymax_s = s->y + s->h / 2;

	uint16_t xmin_o = o->x - o->w / 2;
	uint16_t xmax_o = o->x + o->w / 2;
	uint16_t ymin_o = o->y - o->h / 2;
	uint16_t ymax_o = o->y + o->h / 2;

	/// Determine which side collision is occuring
	// Collision bottom side
	if ( ymax_s >= ymin_o - ICECUBE_BORDER)
	{
		if ((s->corner[CORNER_BR].x >= xmin_o && s->corner[CORNER_BR].x <= xmax_o && s->corner[CORNER_BR].y <= (ymax_o - o->h / 2) )
		or ( s->corner[CORNER_BL].x >= xmin_o && s->corner[CORNER_BL].x <= xmax_o && s->corner[CORNER_BL].y <= (ymax_o - o->h / 2) )
		or ( o->corner[CORNER_TR].x >= xmin_s && o->corner[CORNER_TR].x <= xmax_s && s->corner[CORNER_BR].y <= (ymax_o - o->h / 2) ) )
				res |= 1 << DIR_BOTTOM;
	}

	// Collision top side
	if ( ymin_s <= ymax_o + ICECUBE_BORDER)
	{
		if ((s->corner[CORNER_TR].x >= xmin_o && s->corner[CORNER_TR].x <= xmax_o && s->corner[CORNER_TR].y >= (ymin_o + o->h / 2) )
		or ( s->corner[CORNER_TL].x >= xmin_o && s->corner[CORNER_TL].x <= xmax_o && s->corner[CORNER_TL].y >= (ymin_o + o->h / 2) )
		or ( o->corner[CORNER_BR].x >= xmin_s && o->corner[CORNER_BR].x <= xmax_s && s->corner[CORNER_TR].y >= (ymin_o + o->h / 2) ) )
				res |= 1 << DIR_TOP;
	}

	// Collision right side
	if ( xmax_s >= xmin_o - ICECUBE_BORDER)
	{
		if ((s->corner[CORNER_BR].y >= ymin_o && s->corner[CORNER_BR].y <= ymax_o && s->corner[CORNER_BR].x <= (xmax_o - o->w / 2) )
		or ( s->corner[CORNER_TR].y >= ymin_o && s->corner[CORNER_TR].y <= ymax_o && s->corner[CORNER_TR].x <= (xmax_o - o->w / 2) )
		or ( o->corner[CORNER_TL].y >= ymin_s && o->corner[CORNER_TL].y <= ymax_s && s->corner[CORNER_BR].x <= (xmax_o - o->w / 2) ) )
				res |= 1 << DIR_RIGHT;
	}

	// Collision left side
	if ( xmin_s <= xmax_o + ICECUBE_BORDER)
	{
		if ((s->corner[CORNER_BL].y >= ymin_o && s->corner[CORNER_BL].y <= ymax_o && s->corner[CORNER_BL].x >= (xmin_o + o->w / 1.5) )
		or ( s->corner[CORNER_TL].y >= ymin_o && s->corner[CORNER_TL].y <= ymax_o && s->corner[CORNER_TL].x >= (xmin_o + o->w / 1.5) )
		or ( o->corner[CORNER_TR].y >= ymin_s && o->corner[CORNER_TR].y <= ymax_s && s->corner[CORNER_TL].x >= (xmin_o + o->w / 1.5) ) )
				res |= 1 << DIR_LEFT;
	}

	return res;
}


/////////////////// MISC METHODS ///////////////////////
////////////////////////////////////////////////////////

void Engine::playerInput(uint8_t p)
{
	Player* s = &player[p];

    if ( !getInput(P_IN[p][P_ACTION]) ){
    	s->dash_btnState = false;
    }

    if (s->dashCooldown > 0)
    {
    	s->dashCooldown -= getTime(tick_clk);
    	if (s->dashCooldown < 0) s->dashCooldown = 0;
    }

	if (s->hasDashed)
		s->dashTimer += getTime(tick_clk);

	if (s->dashTimer > 100)
	{
		s->hasDashed = 0;
		s->dashTimer = 0;
	}

	if (s->hasDashed == false)
	{
	    if ( getInput(P_IN[p][DIR_TOP]) )
	    {
	    	s->setY_spd(-PLAYER_SPD);
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_TOP );
	    }
	    else if ( getInput(P_IN[p][DIR_BOTTOM]) )
	    {
	    	s->setY_spd(PLAYER_SPD);
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_BOTTOM );
	    }
	    else
	    {
	    	s->setY_spd(0);
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_BOTTOM );
	    }

	    if ( getInput(P_IN[p][DIR_LEFT]) )
	    {
	    	s->setX_spd(-PLAYER_SPD);
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_LEFT );
	    }
	    else if ( getInput(P_IN[p][DIR_RIGHT]) )
	    {
	    	s->setX_spd(PLAYER_SPD);
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_RIGHT );
	    }
	    else
	    {
	    	s->setX_spd(0);
	    }

	    if (s->x_spd < 0)
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_LEFT );
	    else if (s->x_spd > 0)
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_RIGHT );
	    else if (s->y_spd < 0)
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_TOP );
	    else if (s->y_spd > 0)
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_BOTTOM );
	    else
	    	s->setId( (PLAYER_ID + 4 * p) + DIR_BOTTOM );
	}

    if (getInput(P_IN[p][P_ACTION]) && (player[p].dash_btnState == false) && (player[p].dashCooldown == 0) )
    {
    		player[p].dash_btnState = true;
    		player[p].hasDashed = true;
    		player[p].dashCooldown = PLAYER_DASH_CD;

    		player[p].x_spd *= PLAYER_DASH_SPDUP;
    		player[p].y_spd *= PLAYER_DASH_SPDUP;
    }
}

void Engine::updatePlayer(IObject* p)
{
	uint16_t ORX = p->x;
	uint16_t ORY = p->y;
	uint8_t ORID = p->id;

	uint8_t temp_id = 0;

	if (p == &player[0])
		temp_id = PLAYER_ID;
	else
		temp_id = PLAYER_ID + 4;

	for (int i = temp_id; i < temp_id + 4; i++)
	{
		if (i != ORID)
		{
			p->id = i;
			p->x = 0;
			p->y = 0;

			spi.send_gfx_packet(*p);
		}
	}

	p->id = ORID;
	p->x = ORX;
	p->y = ORY;
}

void Engine::checkHealth(Player* p)
{
	IObject* g;

	if (p == &player[0])
	{
		g = &graphic[p->hp];
		g->x = XMIN - 15;
		spi.send_gfx_packet(*g);
	}
	else if (p == &player[1])
	{
		g = &graphic[p->hp + 3];
		g->x = XMIN - 15;
		spi.send_gfx_packet(*g);
	}
}

void Engine::snowballSpawner()
{
	srand( rand() % rand_seed );

	Snowball* s;

	for (int i = 0; i < SNOWBALL_COUNT; i++)
	{
		s = &snowball[i];

		if (!s->en)
			continue;

		if (s->respawn_timer > 0)
			s->respawn_timer -= getTime(tick_clk);
		else if (s->respawn_timer < 0)
			s->respawn_timer = 0;

		if (s->respawn_timer == 0 && s->spawned == 0)
		{
			snowballRandomise(s);
			s->spawned = 1;
		}

		if ( s->en )
		{
			if ( (s->x < XMIN ||
				 s->x > XMAX ||
				 s->y < YMIN ||
				 s->y > YMAX ) && s->spawned == 1)
			{
				s->spawned = 0;
				s->x = XMIN - 15;
				s->respawn_timer = rand()%(snowball_spawn_max - snowball_spawn_min + 1) + snowball_spawn_min;
//				snowballRandomise(&snowball[i]);
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
		s->setY( YMIN + 2 );
		s->setX_spd(0);
		s->setY_spd( rand() % 100 + MIN_SPD);
		break;

	case 1:
		s->setX( XMAX - 2 );
		s->setY( rand() % (YMAX - YMIN) + YMIN);
		s->setX_spd( -(rand() % 100) - MIN_SPD);
		s->setY_spd( 0 );
		break;

	case 2:
		s->setX( rand() % (XMAX - XMIN) + XMIN);
		s->setY( YMAX - 2 );
		s->setX_spd(0);
		s->setY_spd( -(rand() % 100) - MIN_SPD);
		break;

	case 3:
		s->setX( XMIN + 2 );
		s->setY( rand() % (YMAX - YMIN) + YMIN);
		s->setX_spd( rand() % 100 + MIN_SPD);
		s->setY_spd( 0 );
		break;
	}
}

void Engine::despawnIcecubes()
{
	uint16_t xmin, xmax, ymin, ymax;

	for (int i = 0; i < ICECUBE_COUNT; i++)
	{
		IObject* s = &icecube[i];

		if (!s->en)
			continue;

		xmin = s->x - s->w / 2;
		xmax = s->x + s->w / 2;
		ymin = s->y - s->h / 2;
		ymax = s->y + s->h / 2;

		if (xmax < XMIN_OUT || xmin > XMAX_OUT || ymax < YMIN_OUT || ymin > YMAX_OUT)
		{
//			s->x_spd = 0;
//			s->y_spd = 0;
			s->x = 0;
			s->y = 0;
			continue;
		}

		if (xmin < XMIN_OUT)
			s->x_spd = -PLAYER_SPD * 0.5;
		else if (xmax > XMAX_OUT)
			s->x_spd = PLAYER_SPD * 0.5;

		if (ymin < YMIN_OUT)
			s->y_spd = -PLAYER_SPD * 0.5;
		else if (ymax > YMAX_OUT)
			s->y_spd = PLAYER_SPD * 0.5;
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

bool Engine::getInput(IO_PORT port)
{
    return HAL_GPIO_ReadPin(port.port, port.pin);
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
