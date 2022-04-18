/*
* SPI.h
*
*  Created on: Feb 15, 2022
*      Author: timvd
*/

#ifndef SRC_SERIAL_H_
#define SRC_SERIAL_H_

// UART debug //
#include <debug.h>

extern UART_HandleTypeDef huart2;
////////////////

#include <main.h>
#include <stdint.h>

class IObject;

class SPI {

public:
	SPI();
	virtual ~SPI();

public:
	void send_gfx_packet(const IObject& object);
	void send_gfx_packet(bool en, uint8_t id, uint16_t x, uint16_t y);
	void send_confirm();

private:
	uint8_t sck_cycle = 0;

	const int packet_gfx_size = 28;
	uint32_t packet_gfx = 0;
};

#endif /* SRC_SERIAL_H_ */
