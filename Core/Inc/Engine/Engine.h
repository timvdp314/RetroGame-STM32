/*
* Engine.h
*
*  Created on: 20 Feb 2022
*      Author: timvd
*/

#ifndef INC_ENGINE_ENGINE_H_
#define INC_ENGINE_ENGINE_H_

// UART debug //
#include <debug.h>

extern UART_HandleTypeDef huart2;
////////////////

		//Name	   //GPIO pin number      //Pin on STM32		
#define P1_UP_PIN       GPIO_PIN_9  //PA9  		    D8  			      
#define P1_DOWN_PIN     GPIO_PIN_7  //PC7 		    D9  			      
#define P1_LEFT_PIN     GPIO_PIN_6  //PB6  		    D10					  
#define P1_RIGHT_PIN    GPIO_PIN_7  //PA7  		    D11					 

#define P1_UP_PORT      GPIOA
#define P1_DOWN_PORT    GPIOC
#define P1_LEFT_PORT    GPIOB
#define P1_RIGHT_PORT   GPIOA

#include <Engine/Game-entities/Player.h>
#include <Engine/Game-entities/Snowball.h>
#include <Engine/Game-entities/Powerup.h>
#include <Engine/Game-entities/Icecube.h>

#include <Engine/Interface/SPI.h>

#define REFRESH_PRESCALER 20

class Engine {
public:
	Engine();
	Engine(TIM_HandleTypeDef* refresh_clk,
		   TIM_HandleTypeDef* tick_clk);
	virtual ~Engine();

// Core methods
public:
	void gameLoop();
	void gameUpdate();
	void screenUpdate();

// Helper methods
public:
	uint16_t getTime(TIM_HandleTypeDef* clk);
	void setTime(TIM_HandleTypeDef* clk, uint16_t time);
    bool getInput(GPIO_TypeDef* port, uint16_t pin);

// Game entities
private:
	Player player[2];
	Snowball snowball[20];
	Powerup powerup [3];
	Icecube icecube [10];


// Interface
private:
	SPI spi;


// Timer references
private:
	TIM_HandleTypeDef* refresh_clk;
	TIM_HandleTypeDef* tick_clk;


// Constants
private:
	const uint8_t PLAYER_COUNT = 2;
	const uint8_t SNOWBALL_COUNT = 20;
	const uint8_t POWERUP_COUNT = 3;
	const uint8_t ICECUBE_COUNT = 10;

	//Starting id's for each entity
	//Id's are in order of player -> snowball -> powerup -> icecube
	const uint8_t PLAYER_ID = 1;
	const uint8_t SNOWBALL_ID = PLAYER_ID + PLAYER_COUNT;
	const uint8_t POWERUP_ID = SNOWBALL_ID + SNOWBALL_COUNT;
	const uint8_t ICECUBE_ID = POWERUP_ID + POWERUP_COUNT;
};

#endif /* INC_ENGINE_ENGINE_H_ */
