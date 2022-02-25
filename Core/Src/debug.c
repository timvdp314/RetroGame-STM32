/*
 * debug.c
 *
 *  Created on: Feb 22, 2022
 *      Author: timvd
 */

#include <debug.h>

// Print via UART terminal
void debugPrint(char _out[]){
	HAL_UART_Transmit(&huart2, (uint8_t *) _out, strlen(_out), 10);
}

void debugPrintln(char _out[]){
	HAL_UART_Transmit(&huart2, (uint8_t *) _out, strlen(_out), 10);
	char newline[4] = "\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t *) newline, 2, 10);
}

//USAGE EXAMPLE
// char str1[20];
// sprintf(str1, "%d", x);
// debugPrintln( str1 );

// Convert int to binary representation
char *int2bin(int a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

//USAGE EXAMPLE
// char buffer[33];
// buffer[33 - 1] = '\0';
// int2bin(x, buffer, 33 - 1);
// debugPrintln(buffer);
