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
		player[i].setId(PLAYER_ID + i);

	for (int i = 0; i < SNOWBALL_COUNT; i++)
		snowball[i].setId(SNOWBALL_ID + i);

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

	snowball[1].setX_spd(100);
	snowball[1].setY(200);
	snowball[1].setX(300);
	snowball[1].setEnabled(0);

	snowball[2].setY_spd(80);
	snowball[2].setX_spd(-30);
	snowball[2].setY(200);
	snowball[2].setX(300);
	snowball[2].setEnabled(1);
}

Engine::~Engine() {
	// TODO Auto-generated destructor stub
}

////// Core methods //////
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

	if ( getTime(tick_clk) )
	{
		player[0].updatePos();
//		snowball[0].updatePos();
//		snowball[1].updatePos();
//		snowball[2].updatePos();
		setTime(tick_clk, 0);
	}
}

void Engine::screenUpdate()
{
//	for (Player p : player)
//		if ( p.hasChanged() )
//		{
//			p.updateChanges();
//			spi.send_gfx_packet(p);
//		}

	if (player[0].hasChanged())
	{
		player[0].updateChanges();
		spi.send_gfx_packet(player[0]);
	}

//	for (Snowball s : snowball)
//		if ( s.hasChanged() )
//		{
//			s.updateChanges();
//			spi.send_gfx_packet(s);
//		}

//	spi.send_gfx_packet(snowball[0]);
//	spi.send_gfx_packet(snowball[1]);
//	spi.send_gfx_packet(snowball[2]);

	spi.send_confirm();
}

//////////////////////////

// Helper methods
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
