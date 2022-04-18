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

//Name	   //GPIO pin number      //Pin on STM32		//Pin on Basys 3 (JB connector)
#define SPI_SCK_PIN    GPIO_PIN_10  //PA10  		    D2			      A14 (top-right)
#define SPI_MOSI_PIN   GPIO_PIN_3  //PB3 		    D3  			  A16 (left of A14)
#define SPI_CS0_PIN    GPIO_PIN_5  //PB5  		    D4				  B15 (left of A16)
#define SPI_CS1_PIN    GPIO_PIN_4 //PB4  		    D5				  B16 (left of B15)

#define SPI_SCK_PORT   GPIOA
#define SPI_MOSI_PORT  GPIOB
#define SPI_CS0_PORT   GPIOB
#define SPI_CS1_PORT   GPIOB

//Name	   			 //GPIO pin number      //Pin on STM32
#define P1_LEFT_PIN     GPIO_PIN_9  //PA9  		 D8
#define P1_DOWN_PIN     GPIO_PIN_7  //PC7 		 D9
#define P1_UP_PIN       GPIO_PIN_6  //PB6  		 D10
#define P1_RIGHT_PIN    GPIO_PIN_7  //PA7  		 D11
#define P1_ACTION_PIN	GPIO_PIN_8 //PA8		 D6

#define P1_LEFT_PORT    GPIOA
#define P1_DOWN_PORT    GPIOC
#define P1_UP_PORT      GPIOB
#define P1_RIGHT_PORT   GPIOA
#define P1_ACTION_PORT  GPIOA

//Name	   		   / /GPIO pin number      //Pin on STM32
#define P2_LEFT_PIN     GPIO_PIN_6  //PA6  		 D12
#define P2_DOWN_PIN     GPIO_PIN_5  //PA5 		 D13
#define P2_UP_PIN       GPIO_PIN_9  //PB9 		 D14
#define P2_RIGHT_PIN    GPIO_PIN_8  //PB8  		 D15
#define P2_ACTION_PIN	GPIO_PIN_10  //PB10		 D7

#define P2_LEFT_PORT    GPIOA
#define P2_DOWN_PORT    GPIOA
#define P2_UP_PORT      GPIOB
#define P2_RIGHT_PORT   GPIOB
#define P2_ACTION_PORT  GPIOB

#include <Engine/Game-entities/Player.h>
#include <Engine/Game-entities/Snowball.h>
#include <Engine/Game-entities/Icecube.h>
#include <Engine/Game-entities/Graphic.h>

#include <Engine/Interface/SPI.h>

#define DIR_RIGHT 0
#define DIR_TOP 1
#define DIR_LEFT 2
#define DIR_BOTTOM 3
#define P_ACTION 4

enum class SPR{
	PLAYER,
	SNOWBALL,
	ICECUBE,
	GRAPHIC
};

enum class GAME_STATE{
	MENU,
	PLAY
};

struct IO_PORT
{
	GPIO_TypeDef* port;
	uint16_t pin;
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
	void gameLoop();
	void menuLoop();

	void gameInit();
	void gameUpdate();
	void spritesUpdate();

	void screenUpdate();
	void screenClear();

	void gameOver();

// Sprite functions
	// Movement
	void spritePosUpdate(IObject*);
	void spriteSubpixUpdate(IObject*);

	// Collision
	void checkCollision(IObject*);
	bool checkAnyCollision(IObject*, IObject*);
	uint8_t checkSidesCollision(IObject*, IObject*);

	// Snowball spawner
	void snowballSpawner();
	void snowballRandomise(IObject*);

	// Icecube
	void despawnIcecubes();

// Misc
	void playerInput(uint8_t p);
	void updatePlayer(IObject* p);
	void checkHealth(Player* p);

// Helper methods
public:
	uint16_t getTime(TIM_HandleTypeDef* clk);
	void setTime(TIM_HandleTypeDef* clk, uint16_t time);
    bool getInput(IO_PORT port);

// Debug methods
public:
    void checkRefreshRate();
    void printSpriteInfo(IObject* s);


/// -- Attributes -- ///

// Core attributes
private:
    GAME_STATE state = GAME_STATE::MENU;
    bool game_init = 1;

    uint16_t snowball_spawn_min = 1000;
    uint16_t snowball_spawn_max = 3000;

// Game entities
private:
	Player player[2];
	Snowball snowball[5];
	Icecube icecube[3];
	Graphic graphic[7];

// Interface
private:
	SPI spi;


// Timer references
private:
	TIM_HandleTypeDef* refresh_clk;
	TIM_HandleTypeDef* tick_clk;
	TIM_HandleTypeDef* debug_clk;

// IO Ports
private:
	const IO_PORT P_IN[2][5] = {
			{ { P1_RIGHT_PORT,  P1_RIGHT_PIN },
			{ P1_UP_PORT,    P1_UP_PIN },
			{ P1_LEFT_PORT,   P1_LEFT_PIN },
			{ P1_DOWN_PORT, P1_DOWN_PIN },
			{ P1_ACTION_PORT, P1_ACTION_PIN } },

			{ { P2_RIGHT_PORT,  P2_RIGHT_PIN },
			{ P2_UP_PORT,    P2_UP_PIN },
			{ P2_LEFT_PORT,   P2_LEFT_PIN },
			{ P2_DOWN_PORT, P2_DOWN_PIN },
			{ P2_ACTION_PORT, P2_ACTION_PIN } }
	};

// Constants
private:
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

	// Entity count
	const uint8_t PLAYER_COUNT = 2;
	const uint8_t SNOWBALL_COUNT = 5;
	const uint8_t ICECUBE_COUNT = 3;
	const uint8_t GRAPHIC_COUNT = 7;

	// Starting id's for each entity
	// Id's are in order of player -> snowball -> powerup -> icecube
	const uint8_t PLAYER_ID = 1;
	const uint8_t SNOWBALL_ID = PLAYER_ID + PLAYER_COUNT * 4;
	const uint8_t ICECUBE_ID = SNOWBALL_ID + SNOWBALL_COUNT;
	const uint8_t GRAPHIC_ID = ICECUBE_ID + ICECUBE_COUNT;

	// Snowball data
	const uint8_t MIN_SPD = 175;

	// Sprite data
	const uint8_t PLAYER_W = 46;
	const uint8_t PLAYER_H = 50;
	const uint8_t PLAYER_SPD = 125;
	const uint8_t PLAYER_WALK_SPD = 75;
	const uint8_t PLAYER_DASH_SPDUP = 5;
	const int16_t PLAYER_DASH_CD = 1000;
	const uint16_t PLAYER1_STARTX = (XMIN + XMAX) / 2 - PLAYER_W * 1.25;
	const uint16_t PLAYER1_STARTY = (YMIN + YMAX) / 2 - 50;
	const uint16_t PLAYER2_STARTX = (XMIN + XMAX) / 2 - PLAYER_W * 1.25;
	const uint16_t PLAYER2_STARTY = (YMIN + YMAX) / 2 + 50;

	const uint8_t SNOWBALL_W = 20;
	const uint8_t SNOWBALL_H = 20;

	const uint8_t LIFE_W = 12;
	const uint8_t LIFE_H = 12;
	const uint8_t LIFE_XDIST = 30;
	const uint8_t LIFE_YDIST = 30;

	// Misc
	uint32_t rand_seed = 0;

	const uint8_t ICECUBE_BORDER = 3;

	// Debug variables
	uint8_t frame = 0;
	uint16_t clk_t = 0;
};

#endif /* INC_ENGINE_ENGINE_H_ */
