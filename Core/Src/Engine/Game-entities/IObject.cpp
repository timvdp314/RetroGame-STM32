/*
* IObject.cpp
*
*  Created on: 21 Feb 2022
*      Author: timvd
*/

#include <Engine/Game-entities/IObject.h>
#include <Engine/Engine.h>

IObject::IObject() {
	// TODO Auto-generated constructor stub
}

IObject::~IObject() {
	// TODO Auto-generated destructor stub
}

//// Getter / setter methods ////
void IObject::setX(uint16_t x) 		   	{ this->x = x; }
void IObject::setY(uint16_t y) 		   	{ this->y = y; }
void IObject::setX_sub(int16_t x_sub)   { this->x_sub = x_sub; }
void IObject::setY_sub(int16_t y_sub)   { this->y_sub = y_sub; }
void IObject::setW(uint8_t w) 		   	{ this->w = w; }
void IObject::setH(uint8_t h) 		   	{ this->h = h; }
void IObject::setX_spd(int16_t x_spd)   { this->x_spd = x_spd; }
void IObject::setY_spd(int16_t y_spd)   { this->y_spd = y_spd; }
void IObject::setEnabled(bool enabled) 	{ this->en = enabled; }
void IObject::setId(uint8_t id) 	   	{ this->id = id; }

uint16_t IObject::getX() const    	   	{ return x; }
uint16_t IObject::getY() const         	{ return y; }
int16_t IObject::setX_sub() 			{ return x_sub; }
int16_t IObject::setY_sub()   			{ return y_sub; }
uint8_t IObject::getW() const    	   	{ return w; }
uint8_t IObject::getH() const         	{ return h; }
int16_t IObject::getX_spd() const       { return x_spd; }
int16_t IObject::getY_spd() const       { return y_spd; }
bool IObject::getEnabled() const       	{ return en; }
uint8_t IObject::getId() const			{ return id; }
//////////////////////////////////

void IObject::updatePos()
{
	x_tick += x_spd;
	y_tick += y_spd;

	if (x_tick > TICK_MAX)
	{
		x++;
		x_tick %= TICK_MAX;
	}
	else if (x_tick < 0)
	{
		x--;
		x_tick = TICK_MAX;
	}

	if (y_tick > TICK_MAX)
	{
		y++;
		y_tick %= TICK_MAX;
	}
	else if (y_tick < 0)
	{
		y--;
		y_tick = TICK_MAX;
	}
}

bool IObject::hasChanged()
{
	if ( x != x_prev || y != y_prev || en != en_prev )
	{
		x_prev = x;
		y_prev = y;
		en_prev = en;

		return true;
	}
	else
		return false;
}
