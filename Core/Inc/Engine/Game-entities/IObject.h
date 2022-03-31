/*
* IObject.h
*
*  Created on: 21 Feb 2022
*      Author: timvd
*/

#ifndef INC_ENGINE_GAME_ENTITIES_IOBJECT_H_
#define INC_ENGINE_GAME_ENTITIES_IOBJECT_H_

// UART debug //
#include <debug.h>

extern UART_HandleTypeDef huart2;
////////////////

#include <stdint.h>

#define CHECK_ANY 0
#define CHECK_X 1
#define CHECK_Y 2

#define FREE_RIGHT 0
#define FREE_TOP 1
#define FREE_LEFT 2
#define FREE_BOTTOM 3

#define CORNER_TR 0
#define CORNER_TL 1
#define CORNER_BL 2
#define CORNER_BR 3

class Engine;
enum class SPR;

struct COORD
{
	uint16_t x;
	uint16_t y;
};

class IObject {
public:
	IObject();
	virtual ~IObject();

//Getter / setter methods
public:
	virtual void setX(uint16_t x);
	virtual void setY(uint16_t y);
	virtual void setX_sub(int16_t x_sub);
	virtual void setY_sub(int16_t y_sub);
	virtual void setW(uint8_t w);
	virtual void setH(uint8_t h);
	virtual void setX_spd(int16_t x_spd);
	virtual void setY_spd(int16_t y_spd);
	virtual void setEnabled(bool enabled);
	virtual void setId(uint8_t id);
	virtual void setType(SPR type);

	virtual uint16_t getX() const;
	virtual uint16_t getY() const;
	virtual int16_t getX_sub() const;
	virtual int16_t getY_sub() const;
	virtual uint8_t getW() const;
	virtual uint8_t getH() const;
	virtual int16_t getX_spd() const;
	virtual int16_t getY_spd() const;
	virtual bool getEnabled() const;
	virtual uint8_t getId() const;
	virtual SPR getType() const;

public:
	virtual bool hasChanged(uint8_t = CHECK_ANY);

protected:
	uint8_t id = 0;
	SPR type;

	bool en = 0;
	uint16_t x = 0;
	uint16_t y = 0;

	int16_t x_sub = 0;
	int16_t y_sub = 0;
	uint8_t w = 0;
	uint8_t h = 0;

	bool en_prev = 0;
	uint16_t x_prev = 0;
	uint16_t y_prev = 0;

	int16_t x_spd = 0;
	int16_t y_spd = 0;

	COORD corner[4];
	bool isFree[4] = {1, 1, 1, 1};

	const uint8_t spr_border = 3;

	const uint16_t TICK_MAX = 1000;

	friend class Engine;
};

#endif /* INC_ENGINE_GAME_ENTITIES_IOBJECT_H_ */
