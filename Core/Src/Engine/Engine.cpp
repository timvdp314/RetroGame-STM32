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
			   TIM_HandleTypeDef* tick_clk)
{
	//Screen refresh clock
	this->refresh_clk = refresh_clk;
	this->tick_clk = tick_clk;

	// Assign object ID's
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

	player[0].setX(400);
	player[0].setY(300);
	player[0].setEnabled(1);

	snowball[0].setY_spd(75);
	snowball[0].setY(200);
	snowball[0].setX(200);
	snowball[0].setEnabled(1);

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
	snowball[3].setX(100);
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
	gameUpdate();

	if ( getTime(refresh_clk) > REFRESH_PRESCALER)
	{
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
	uint8_t collision = isColliding(spr);

	if ( collision & (1 << 0) || collision & (1 << 1) ) spr->setX_spd(0);
	if ( collision & (1 << 2) || collision & (1 << 3) ) spr->setY_spd(0);

	spr->updatePos();

	// Check for sprite wrap
	if 	 	(spr->getX() > XMAX) spr->setX(XMIN);
	else if (spr->getX() < XMIN) spr->setX(XMAX);

	if 	 	(spr->getY() > YMAX) spr->setY(YMIN);
	else if (spr->getY() < YMIN) spr->setY(YMAX);
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
	if (ymax >= YMAX) res |= 1 << 2; // Collision on top side
	if (ymin <= YMIN) res |= 1 << 3; // Collision on bottom side

	return res;
}

/////////////////// MISC METHODS ///////////////////////
////////////////////////////////////////////////////////

void Engine::playerInput()
{
    if (getInput(P1_UP_PORT, P1_UP_PIN))
    {
        player[0].setY_spd(-50);
    }
    else if (getInput(P1_DOWN_PORT, P1_DOWN_PIN))
    {
        player[0].setY_spd(50);
    }
    else if (getInput(P1_LEFT_PORT, P1_LEFT_PIN))
    {
        player[0].setX_spd(-50);
    }
    else if (getInput(P1_RIGHT_PORT, P1_RIGHT_PIN))
        player[0].setX_spd(50);
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
