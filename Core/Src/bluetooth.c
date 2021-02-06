/*
 * bluetooth.c
 *
 *  Created on: Nov 16, 2020
 *      Author: micael
 */
#include "bluetooth.h"
#include "gpio.h"

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */



/*void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  //Receive Data register not empty interrupts
  //__HAL_UART_ENABLE(&huart1);
  //__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  //__HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}*/
void UART1_GPIO_CFG(void)
{
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**USART1 GPIO Configuration
	PA9     ------> USART1_TX
	PA10     ------> USART1_RX
	*/

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void NVIC_Configuration(void)
{
	//Configure the NVIC Preemption Priority Bits
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

}
void UART1_IRQHandler(void)
{
	memset(R_msg, 0, sizeof(R_msg));
	HAL_UART_Receive(&huart1, (uint8_t*)&R_msg[0],1,20);
}







