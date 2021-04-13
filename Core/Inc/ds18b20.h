/*
 * DS18B20.h
 *
 *  Created on: Nov 14, 2020
 *      Author: micael
 */
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#define DS18B20_PORT GPIOA
#define DS18B20_PIN GPIO_PIN_8

//Functions
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t Start_DS18B20 (void);
void Write_Byte_DS18B20 (uint8_t data);
uint8_t Read_DS18B20 (void);

ADC_HandleTypeDef hadc1;

//Variables
uint8_t DS18B20_ROM_ADDR[2][8];
uint8_t Temp_byte1;
uint8_t Temp_byte2;
uint16_t Temp;
float Temperatures[2];
uint8_t Response;
uint8_t data;
char msg[256];


#endif /* INC_DS18B20_H_ */
