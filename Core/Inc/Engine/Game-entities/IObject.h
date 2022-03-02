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

class Engine;

class IObject {
public:
	IObject();
	virtual ~IObject();

//Getter / setter methods
public:
	virtual void setX(uint16_t x);
	virtual void setY(uint16_t y);
	virtual void setX_spd(int16_t x_spd);
	virtual void setY_spd(int16_t y_spd);
	virtual void setEnabled(bool enabled);
	virtual void setId(uint8_t id);

	virtual uint16_t getX() const;
	virtual uint16_t getY() const;
	virtual int16_t getX_spd() const;
	virtual int16_t getY_spd() const;
	virtual bool getEnabled() const;
	virtual uint8_t getId() const;

public:
	virtual void updatePos();
	virtual bool hasChanged();
	virtual void updateChanges();

protected:
	uint8_t id = 0;

	bool en = 0;
	uint16_t x = 0;
	uint16_t y = 0;

	bool en_prev = 0;
	uint16_t x_prev = 0;
	uint16_t y_prev = 0;

	int16_t x_spd = 0;
	int16_t y_spd = 0;

	int16_t x_tick = 0;
	int16_t y_tick = 0;
	const uint16_t TICK_MAX = 1000;
};

#endif /* INC_ENGINE_GAME_ENTITIES_IOBJECT_H_ */
