/*
* Powerup.h
*
*  Created on: 20 Feb 2022
*      Author: timvd
*/

#ifndef POWERUP_H_
#define POWERUP_H_

#include <stdint.h>
#include <Engine/Game-entities/IObject.h>

class Powerup : public IObject {
public:
	Powerup();
	virtual ~Powerup();
};

#endif /* POWERUP_H_ */
