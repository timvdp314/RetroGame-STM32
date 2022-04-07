/*
* Engine.h
*
*  Created on: 20 Feb 2022
*      Author: timvd
*/

#ifndef INC_ENGINE_ENGINE_H_
#define INC_ENGINE_ENGINE_H_

// Standard libraries //
#include <stdlib.h>

// UART debug //
#include <debug.h>

extern UART_HandleTypeDef huart2;
////////////////

//Name	   //GPIO pin number      //Pin on STM32
#define P1_LEFT_PIN     GPIO_PIN_9  //PA9  		 D8
#define P1_DOWN_PIN     GPIO_PIN_7  //PC7 		 D9
#define P1_UP_PIN       GPIO_PIN_6  //PB6  		 D10
#define P1_RIGHT_PIN    GPIO_PIN_7  //PA7  		 D11
#define P1_ACTION_PIN	GPIO_PIN_10 //PA10		 D2

#define P1_LEFT_PORT    GPIOA
#define P1_DOWN_PORT    GPIOC
#define P1_UP_PORT      GPIOB
#define P1_RIGHT_PORT   GPIOA
#define P1_ACTION_PORT  GPIOA

//Name	   		   //GPIO pin number      //Pin on STM32
#define P2_LEFT_PIN     GPIO_PIN_6  //PA6  		 D12
#define P2_DOWN_PIN     GPIO_PIN_5  //PA5 		 D13
#define P2_UP_PIN       GPIO_PIN_9  //PB9 		 D14
#define P2_RIGHT_PIN    GPIO_PIN_8  //PB8  		 D15
#define P1_ACTION_PIN	GPIO_PIN_10 //PA10		 D7

#define P2_LEFT_PORT    GPIOA
#define P2_DOWN_PORT    GPIOA
#define P2_UP_PORT      GPIOB
#define P2_RIGHT_PORT   GPIOB

#include <Engine/Game-entities/Player.h>
#include <Engine/Game-entities/Snowball.h>
#include <Engine/Game-entities/Powerup.h>
#include <Engine/Game-entities/Icecube.h>

#include <Engine/Interface/SPI.h>

#define COLL_RIGHT 0
#define COLL_TOP 1
#define COLL_LEFT 2
#define COLL_BOTTOM 3

enum class SPR{
	PLAYER,
	SNOWBALL,
	PWUP,
	ICECUBE
};

class Engine {
public:
	Engine();
	Engine(TIM_HandleTypeDef* refresh_clk,
		   TIM_HandleTypeDef* tick_clk,
		   TIM_HandleTypeDef* debug_clk,
		   uint32_t rand_seed);
	virtual ~Engine();

/// -- Methods -- ///
// Core methods
public:
	/**
	 * @brief Test
	 */
	void gameLoop();
	void gameUpdate();
	void spritesUpdate();
	void screenUpdate();

// Sprite functions
	// Movement
	void spritePosUpdate(IObject*);
	void spriteSubpixUpdate(IObject*);

	// Collision
	uint8_t isColliding(IObject*);
	bool checkAnyCollision(IObject*, IObject*);
	uint8_t checkSidesCollision(IObject*, IObject*);

	// Snowball spawner
	void snowballSpawner();
	void snowballRandomise(IObject*);

	// Icecube
	void despawnIcecubes();

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

	// Snowball data
	const uint8_t MIN_SPD = 150;

	// Sprite data
	const uint8_t PLAYER_W = 46;
	const uint8_t PLAYER_H = 50;
	const uint8_t PLAYER_SPD = 125;
	const int16_t PLAYER_DASH_CD = 1000;
	const uint8_t SNOWBALL_W = 20;
	const uint8_t SNOWBALL_H = 20;

	// Screen information
	const uint16_t XMIN = 144;
	const uint16_t XMAX = 784;
	const uint16_t YMIN = 31;
	const uint16_t YMAX = 505;

	// Despawn limits for snowballs and icecubes
	const uint16_t XMIN_OUT = 110; //130
	const uint16_t XMAX_OUT = 800; //800
	const uint16_t YMIN_OUT = 0;  //15
	const uint16_t YMAX_OUT = 509; //531

	const uint16_t TICK_FREQ = 1000;
	const uint16_t SUBPIX_MAX = 1000;

	const uint8_t REFRESH_RATE = 50;
	const uint8_t REFRESH_PRESCALER = 1000 / REFRESH_RATE;

	// Misc
	uint32_t rand_seed = 0;

	const uint8_t ICECUBE_BORDER = 3;

	// Debug variables
	uint8_t frame = 0;
	uint16_t clk_t = 0;
};

#endif /* INC_ENGINE_ENGINE_H_ */
