/*
* Player.h
*
*  Created on: 20 Feb 2022
*      Author: timvd
*/

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include <stdint.h>
#include <Engine/Game-entities/IObject.h>

class Player : public IObject {
public:
	Player();
	virtual ~Player();

public:
	void moveRelative(uint8_t xdiff, uint8_t ydiff);
	void moveAbsolute(uint16_t x, uint16_t y);
};

#endif /* SRC_PLAYER_H_ */
