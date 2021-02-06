/*
 * bluetooth.h
 *
 *  Created on: Nov 16, 2020
 *      Author: micael
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

//Bluetooth transmission
#define UART1_GPIO GPIOA
#define UART1_TxPin GPIO_PIN_9
#define UART1_RxPin GPIO_PIN_10
#define USART1_CLK         RCC_APB1Periph_USART1
#define USART1_GPIO_CLK    RCC_APB2Periph_GPIOA
#define USART1_IRQHandler  USART1_IRQHandler
#define PreemptPriority 1
#define SubPriority 0
#define IRQn 1

//void MX_USART1_UART_Init(void);
void UART1_GPIO_CFG(void);
void NVIC_Configuration(void);
void UART1_IRQHandler(void);
UART_HandleTypeDef huart1;

//Bluetooth transmission
char R_msg[32];
char T_msg[32];
uint8_t buffer_index;
HAL_StatusTypeDef ret;
/*
char* p_char BUF_TX_UART1_OUT ;
char* p_char BUF_TX_UART1_IN;
char* p_char BUF_TX_UART1;
int BUF_TX_SIZE;
char *p_char BUF_RX_UART1_OUT;
char *p_char BUF_RX_UART1_IN;
char *p_char BUF_RX_UART1;
int BUF_RX_SIZE;
char TX_UART1_RESTART_FLAG;
char BUF_TX_UART1[BUF_TX_SIZE];
char BUF_RX_UART1[BUF_RX_SIZE];
*/

#endif /* BLUETOOTH_H_ */
