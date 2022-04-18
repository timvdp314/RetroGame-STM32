/*
* SPI.cpp
*
*  Created on: Feb 15, 2022
*      Author: timvd
*/

#include <Engine/Interface/SPI.h>

#include <Engine/Game-entities/IObject.h>
#include <Engine/Engine.h>
#include <main.h>

SPI::SPI() {

}

SPI::~SPI() {

}

void SPI::send_gfx_packet(const IObject& object)
{
	HAL_GPIO_WritePin(SPI_CS0_PORT, SPI_CS0_PIN, GPIO_PIN_RESET);

	uint16_t x = object.getX();
	uint16_t y = object.getY();
	uint8_t id = object.getId();
	bool enabled = object.getEnabled();

    packet_gfx = 0;
    packet_gfx |= x;
	packet_gfx |= y << 10;
	packet_gfx |= id << 20;
	packet_gfx |= enabled << 27;

//	char str1[20];
//	sprintf(str1, "%d", id);
//	debugPrint( str1 );
//	debugPrint( " " );
//	sprintf(str1, "%d", y);
//	debugPrint( str1 );
//	debugPrint( " " );
//	sprintf(str1, "%d", x);
//	debugPrintln( str1 );
////
//	char buffer[33];
//	buffer[33 - 1] = '\0';
//	int2bin(packet_gfx, buffer, 33 - 1);
//	debugPrintln(buffer);

	while (sck_cycle < packet_gfx_size)
	{
		bool bit = (packet_gfx >> sck_cycle) & 1 ;

		if (bit == 1)
			HAL_GPIO_WritePin(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_RESET);

		sck_cycle += 1;
	}

	sck_cycle = 0;
	HAL_GPIO_WritePin(SPI_CS0_PORT, SPI_CS0_PIN, GPIO_PIN_SET);
}

void SPI::send_gfx_packet(bool en, uint8_t id, uint16_t x, uint16_t y)
{
	HAL_GPIO_WritePin(SPI_CS0_PORT, SPI_CS0_PIN, GPIO_PIN_RESET);

    packet_gfx = 0;
    packet_gfx |= x;
	packet_gfx |= y << 10;
	packet_gfx |= id << 20;
	packet_gfx |= en << 27;

//	char str1[20];
//	sprintf(str1, "%d", id);
//	debugPrint( str1 );
//	debugPrint( " " );
//	sprintf(str1, "%d", y);
//	debugPrint( str1 );
//	debugPrint( " " );
//	sprintf(str1, "%d", x);
//	debugPrintln( str1 );
////
//	char buffer[33];
//	buffer[33 - 1] = '\0';
//	int2bin(packet_gfx, buffer, 33 - 1);
//	debugPrintln(buffer);

	while (sck_cycle < packet_gfx_size)
	{
		bool bit = (packet_gfx >> sck_cycle) & 1 ;

		if (bit == 1)
			HAL_GPIO_WritePin(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_RESET);

		sck_cycle += 1;
	}

	sck_cycle = 0;
	HAL_GPIO_WritePin(SPI_CS0_PORT, SPI_CS0_PIN, GPIO_PIN_SET);
}

void SPI::send_confirm()
{
	HAL_GPIO_WritePin(SPI_CS0_PORT, SPI_CS0_PIN, GPIO_PIN_RESET);

	while (sck_cycle < packet_gfx_size)
	{
		HAL_GPIO_WritePin(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_RESET);

		sck_cycle += 1;
	}

	sck_cycle = 0;
	HAL_GPIO_WritePin(SPI_CS0_PORT, SPI_CS0_PIN, GPIO_PIN_SET);
}
