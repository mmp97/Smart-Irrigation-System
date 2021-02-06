/*
 * uart.h
 *
 *  Created on: Nov 16, 2020
 *      Author: micael
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

#include "stm32f4xx_hal.h"
#include "main.h"

void MX_USART2_UART_Init(void);
UART_HandleTypeDef huart2;

//UART variables
uint16_t val;
uint16_t raw;
char buf[32];
char msg[256];


#endif /* SRC_UART_H_ */
