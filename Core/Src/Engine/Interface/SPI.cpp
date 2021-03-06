/*
* SPI.cpp
*
*  Created on: Feb 15, 2022
*      Author: timvd
*/

#include <Engine/Interface/SPI.h>

#include <Engine/Game-entities/IObject.h>
#include <main.h>

SPI::SPI() {

}

SPI::~SPI() {

}

void SPI::send_gfx_packet(const IObject& object)
{
	HAL_GPIO_WritePin(GPIOB, SPI_CS0, GPIO_PIN_RESET);

	uint16_t x = object.getX();
	uint16_t y = object.getY();
	uint8_t id = object.getId();
	bool enabled = object.getEnabled();

    packet_gfx = 0;
    packet_gfx |= x;
	packet_gfx |= y << 10;
	packet_gfx |= id << 20;
	packet_gfx |= enabled << 27;

	while (sck_cycle < packet_gfx_size)
	{
		bool bit = (packet_gfx >> sck_cycle) & 1 ;

		if (bit == 1)
			HAL_GPIO_WritePin(GPIOB, SPI_MOSI, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOB, SPI_MOSI, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIOB, SPI_SCK, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, SPI_SCK, GPIO_PIN_RESET);

		sck_cycle += 1;
	}

	sck_cycle = 0;
	HAL_GPIO_WritePin(GPIOB, SPI_CS0, GPIO_PIN_SET);
}
