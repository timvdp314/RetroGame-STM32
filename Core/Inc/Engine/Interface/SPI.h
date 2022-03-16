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

		//Name	   //GPIO pin number      //Pin on STM32		//Pin on Basys 3 (JB connector)
#define SPI_SCK    GPIO_PIN_3  //PB5  		    D3  			      A14 (top-right)
#define SPI_MOSI   GPIO_PIN_5  //PB3 		    D4  			      A16 (left of A14)
#define SPI_CS0    GPIO_PIN_4  //PB4  		    D5					  B15 (left of A16)
#define SPI_CS1    GPIO_PIN_10 //PB10  		    D6					  B16 (left of B15)


class IObject;

class SPI {

public:
	SPI();
	virtual ~SPI();

public:
	void send_gfx_packet(const IObject& object);
	void send_confirm();

private:
	uint8_t sck_cycle = 0;

	const int packet_gfx_size = 28;
	uint32_t packet_gfx = 0;
};

#endif /* SRC_SERIAL_H_ */
