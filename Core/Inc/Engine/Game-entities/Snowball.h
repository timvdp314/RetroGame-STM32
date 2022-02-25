/*
* Snowball.h
*
*  Created on: 20 Feb 2022
*      Author: timvd
*/

#ifndef SRC_SNOWBALL_H_
#define SRC_SNOWBALL_H_

#include <stdint.h>
#include <Engine/Game-entities/IObject.h>

class Snowball : public IObject {
public:
	Snowball();
	virtual ~Snowball();
};

#endif /* SRC_SNOWBALL_H_ */
