/*
 * dma.c
 *
 *  Created on: Apr 13, 2021
 *      Author: micael
 */
#include "stm32f4xx_hal.h"
#include "main.h"
/**
 *
  * Enable DMA controller clock
  */

void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

