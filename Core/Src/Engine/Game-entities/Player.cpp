/*
* Player.cpp
*
*  Created on: 20 Feb 2022
*      Author: timvd
*/

#include <Engine/Game-entities/Player.h>

Player::Player() {
	// TODO Auto-generated constructor stub

}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

void Player::moveRelative(uint8_t xdiff, uint8_t ydiff)
{
	this->x += xdiff;
	this->y += ydiff;
}

void Player::moveAbsolute(uint16_t x, uint16_t y)
{
	this->x = x;
	this->y = y;
}
