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

private:
    uint16_t dashTimer = 0;
    int16_t dashCooldown = 0;
    bool dash_btnState = false;
    bool hasDashed = false;

    uint8_t hp = 3;
    uint16_t walk_anim = 0;

    friend class Engine;
};

#endif /* SRC_PLAYER_H_ */
