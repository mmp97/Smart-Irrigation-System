/*
 * dma.h
 *
 *  Created on: Apr 13, 2021
 *      Author: micael
 */

#ifndef INC_DMA_H_
#define INC_DMA_H_

#include "stm32f4xx_hal.h"
#include "main.h"

void MX_DMA_Init(void);

DMA_HandleTypeDef hdma_adc1;


#endif /* INC_DMA_H_ */
