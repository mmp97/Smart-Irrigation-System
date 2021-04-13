/*
 * TIMER.h
 *
 *  Created on: Nov 16, 2020
 *      Author: micael
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include "stm32f4xx_hal.h"
#include "main.h"

void MX_TIM10_Init(void);
void MX_TIM11_Init(void);
void MX_TIM13_Init(void);
void MX_RTC_Init(void);
void delay(uint32_t time);

void get_time(void);
void set_time(void);

TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim13;


RTC_HandleTypeDef hrtc;

uint16_t date[8];
uint16_t time[8];



#endif /* SRC_TIMER_H_ */
