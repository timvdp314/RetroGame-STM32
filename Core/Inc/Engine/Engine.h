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
#define P1_LEFT_PIN     GPIO_PIN_9  //PA9  		    D8
#define P1_DOWN_PIN     GPIO_PIN_7  //PC7 		    D9
#define P1_UP_PIN       GPIO_PIN_6  //PB6  		    D10
#define P1_RIGHT_PIN    GPIO_PIN_7  //PA7  		    D11					 

#define P1_LEFT_PORT    GPIOA
#define P1_DOWN_PORT    GPIOC
#define P1_UP_PORT      GPIOB
#define P1_RIGHT_PORT   GPIOA

#include <Engine/Game-entities/Player.h>
#include <Engine/Game-entities/Snowball.h>
#include <Engine/Game-entities/Powerup.h>
#include <Engine/Game-entities/Icecube.h>

#include <Engine/Interface/SPI.h>

#define REFRESH_PRESCALER 25

#define COLL_RIGHT 1 << 0
#define COLL_LEFT 1 << 1
#define COLL_BOTTOM 1 << 2
#define COLL_TOP 1 << 3

#define SUBPIX_MAX 1000

class Engine {
public:
	Engine();
	Engine(TIM_HandleTypeDef* refresh_clk,
		   TIM_HandleTypeDef* tick_clk,
		   TIM_HandleTypeDef* debug_clk);
	virtual ~Engine();

// Core methods
public:
	void gameLoop();
	void gameUpdate();
	void spritesUpdate();
	void screenUpdate();

// Sprite functions
	void spritePosUpdate(IObject*);
	void spriteSubpixUpdate(IObject*);
	uint8_t isColliding(IObject*);

// Misc
	void playerInput();

// Helper methods
public:
	uint16_t getTime(TIM_HandleTypeDef* clk);
	void setTime(TIM_HandleTypeDef* clk, uint16_t time);
    bool getInput(GPIO_TypeDef* port, uint16_t pin);

// Debug methods
public:
    void checkRefreshRate();
    void printSpriteInfo(IObject* s);


/// -- Attributes -- ///
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
	TIM_HandleTypeDef* debug_clk;


// Constants
private:
	const uint8_t PLAYER_COUNT = 2;
	const uint8_t SNOWBALL_COUNT = 8;
	const uint8_t POWERUP_COUNT = 3;
	const uint8_t ICECUBE_COUNT = 5;

	// Starting id's for each entity
	// Id's are in order of player -> snowball -> powerup -> icecube
	const uint8_t PLAYER_ID = 1;
	const uint8_t SNOWBALL_ID = PLAYER_ID + PLAYER_COUNT;
	const uint8_t POWERUP_ID = SNOWBALL_ID + SNOWBALL_COUNT;
	const uint8_t ICECUBE_ID = POWERUP_ID + POWERUP_COUNT;

	// Sprite data
	const uint8_t PLAYER_W = 46;
	const uint8_t PLAYER_H = 50;
	const uint8_t PLAYER_SPD = 75;
	const uint8_t SNOWBALL_W = 20;
	const uint8_t SNOWBALL_H = 20;

	// Screen information
	const uint16_t XMIN = 150; //130
	const uint16_t XMAX = 770; //800
	const uint16_t YMIN = 40;  //15
	const uint16_t YMAX = 500; //531
};

#endif /* INC_ENGINE_ENGINE_H_ */
