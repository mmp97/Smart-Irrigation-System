/*
 * DS18B20.c
 *
 *  Created on: Nov 14, 2020
 *      Author: micael
 */
#include <ds18b20.h>
#include <stdint.h>
#define PORT_DS18B20 GPIOA
#define PIN_DS18B20 GPIO_PIN_8

//64-bit adresses of the temperature sensors
uint8_t DS18B20_ROM_ADDR[2][8] = { {0x28,0x0,0x2c,0xf3,0x8,0x0,0x0,0x1b},
								  {0x28,0x26,0xbd,0xb,0x3a,0x19,0x1,0x99} };
// Setting selected pin as output
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   GPIO_InitStruct.Pin = GPIO_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
// Setting selected pin as input
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	   GPIO_InitTypeDef GPIO_InitStruct = {0};
	   GPIO_InitStruct.Pin = GPIO_Pin;
	   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	   GPIO_InitStruct.Pull = GPIO_PULLUP;
	   HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
// Starting communication with sensor according to manual
uint8_t Start_DS18B20 (void)
{
	uint8_t Response = 0;
	Set_Pin_Output(PORT_DS18B20, PIN_DS18B20);
	HAL_GPIO_WritePin (PORT_DS18B20, PIN_DS18B20, 0);
	delay (480);
	Set_Pin_Input(PORT_DS18B20, PIN_DS18B20);
	delay (80);
	if (!(HAL_GPIO_ReadPin (PORT_DS18B20, PIN_DS18B20))) Response = 1;
	else Response = -1;
	delay (400);
	if(Response != 1)
	{
		strcat((char*)msg, "ERROR: TEMPERATURE SENSORS NOT DETECTED \r\n");
	}
	return Response;
}
// Writing data to sensor according to manual
void Write_Byte_DS18B20 (uint8_t data){
	for(int i=0; i<8; i++)
	{
		if((data & (1<<i))!=0)
		{
			Set_Pin_Output(PORT_DS18B20, PIN_DS18B20);
			HAL_GPIO_WritePin(PORT_DS18B20, PIN_DS18B20, 0);
			delay(1);
			Set_Pin_Input(PORT_DS18B20, PIN_DS18B20);
			delay(60);
		}
		else
		{
			Set_Pin_Output(PORT_DS18B20, PIN_DS18B20);
			HAL_GPIO_WritePin(PORT_DS18B20, PIN_DS18B20, 0);
			delay(60);
			Set_Pin_Input(PORT_DS18B20, PIN_DS18B20);
		}
	}
}
// Reading data from sensor according to manual
uint8_t Read_DS18B20 (void)
{
	Set_Pin_Input(PORT_DS18B20, PIN_DS18B20);
	uint8_t data = 0;
	for(int i=0; i<8; i++)
	{
		Set_Pin_Output(PORT_DS18B20, PIN_DS18B20);
		HAL_GPIO_WritePin(PORT_DS18B20, PIN_DS18B20, 0);
		delay(2);
		Set_Pin_Input(PORT_DS18B20, PIN_DS18B20);
		if(HAL_GPIO_ReadPin (PORT_DS18B20, PIN_DS18B20) == GPIO_PIN_SET)
		{
			data |= 1<<i;
		}
		delay(60);
	}
	return data;
}


