/*
 * debug.h
 *
 *  Created on: Feb 22, 2022
 *      Author: timvd
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#include <main.h>
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart2;

void debugPrint(char _out[]);
void debugPrintln(char _out[]);
char *int2bin(int a, char *buffer, int buf_size);

#endif /* INC_DEBUG_H_ */
