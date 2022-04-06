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
void IObject::setType(SPR type)			{ this->type = type; }

uint16_t IObject::getX() const    	   	{ return x; }
uint16_t IObject::getY() const         	{ return y; }
int16_t IObject::getX_sub() const		{ return x_sub; }
int16_t IObject::getY_sub() const		{ return y_sub; }
uint8_t IObject::getW() const    	   	{ return w; }
uint8_t IObject::getH() const         	{ return h; }
int16_t IObject::getX_spd() const       { return x_spd; }
int16_t IObject::getY_spd() const       { return y_spd; }
bool IObject::getEnabled() const       	{ return en; }
uint8_t IObject::getId() const			{ return id; }
SPR IObject::getType() const			{ return type; }
//////////////////////////////////

bool IObject::hasChanged(uint8_t coord)
{
	if (coord == CHECK_ANY)
	{
		if ( x != x_prev || y != y_prev || en != en_prev ) // Check for any change
		{
			x_prev = x;
			y_prev = y;
			en_prev = en;

			corner[CORNER_TR].x = x + w / 2;
			corner[CORNER_TR].y = y + h / 2;
			corner[CORNER_TL].x = x - w / 2;
			corner[CORNER_TL].y = y + h / 2;

			corner[CORNER_BL].x = x - w / 2;
			corner[CORNER_BL].y = y - h / 2;
			corner[CORNER_BR].x = x + w / 2;
			corner[CORNER_BR].y = y - h / 2;

			return true;
		}
		else
		{
			return false;
		}
	}
	else if (coord == CHECK_X) // Check for change in X-coordinate
	{
		if ( x != x_prev )
		{
			x_prev = x;

			corner[CORNER_TR].x = x + w / 2;
			corner[CORNER_TL].x = x - w / 2;
			corner[CORNER_BL].x = x - w / 2;
			corner[CORNER_BR].x = x + w / 2;

			return true;
		}
		else
		{
			return false;
		}
	}
	else if (coord == CHECK_Y) // Check for change in Y-coordinate
	{
		if ( y != y_prev )
		{
			y_prev = y;

			corner[CORNER_TR].y = y + h / 2;
			corner[CORNER_TL].y = y + h / 2;
			corner[CORNER_BL].y = y - h / 2;
			corner[CORNER_BR].y = y - h / 2;

			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}
