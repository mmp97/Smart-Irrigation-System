/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "gpio.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"
#include "ds18b20.h"
#include "bluetooth.h"
#include "timer.h"
#include "dma.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/*ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim13;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
*/

/* USER CODE BEGIN PV */

void SystemClock_Config(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_RTC_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM13_Init();
  /* USER CODE BEGIN 2 */
  //Start Timer_10 used to create microsecond delay during temperature sensor communication
  HAL_TIM_Base_Start(&htim10);
  HAL_TIM_Base_Start(&htim11);
  HAL_TIM_Base_Start(&htim13);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t adc_value[2];
  uint32_t raw;
  uint32_t Rx, Lux;
  uint8_t moisture;
  uint8_t state = 0;
  uint8_t irrigation;
  char buffer[120];
  //Start DMA from ADC1
  HAL_ADC_Start_DMA(&hadc1, adc_value, 2);

  set_time();

  while (1)
  {
	  __HAL_TIM_SET_COUNTER(&htim11, 0);
	  //Setting buffer values to zero
	  memset(buffer, 0, sizeof(buffer));
//************************************************LIGHT SENSOR*******************************************************************************
	  //Value of ADC conversion automatically stored through DMA in the buffer
	  raw = adc_value[0];
	  //Calculating approximate value in standard SI units
	  Rx = (19251200 - 4700*raw)/raw;
	  Lux = 16301 - 1596*log(Rx);
	  Lux = 4857612*pow(Rx, -1.02911);
	  sprintf(msg, "%ld ",Lux);
//************************************************MOISTURE SENSOR****************************************************************************
	  //Value of ADC conversion automatically stored through DMA in the buffer
	  raw = adc_value[1];
	  //Rescaling raw value from ADC to represent the percent of moisture
	  moisture = (3396 - raw)/19;
	  sprintf(buffer,"%d ",moisture);
	  strcat(msg, buffer);

//************************************************TEMPERATURE SENSORS*************************************************************************
	  //Stop ADC conversion to prevent interrupts during microseconds delay
	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	  HAL_ADC_Stop(&hadc1);
	  //Start sensor communication
		for(int j = 0;j < 2;j++)
		{
		  //Reset the data line
		  Start_DS18B20();
		  //Matching ROM code
		  Write_Byte_DS18B20 (0x55);
		  //Sending ROM code to the corresponding sensor
		  for(int i = 0; i < 8; i++)
		  {
			  Write_Byte_DS18B20(DS18B20_ROM_ADDR[j][i]);
		  }
		  //Start temperature conversion
		  Write_Byte_DS18B20 (0x44);
		}
		HAL_Delay (100);
		// After conversion read temperature data from both sensors
		for(int j = 0;j < 2;j++)
		{
		  //Reset the data line
		  Start_DS18B20();
		  //Matching ROM code
		  Write_Byte_DS18B20 (0x55);
		  //Sending ROM code to the corresponding sensor
		  for(int i = 0; i<8; i++)
			  {
				  Write_Byte_DS18B20(DS18B20_ROM_ADDR[j][i]);
			  }
		  //Sending data from sensor to Mcu
		  Write_Byte_DS18B20 (0xBE);
		  //Receive 8 bits of temperature data and CRC bit
		  Temp_byte1 = Read_DS18B20();
		  //Receive 8 bits of temperature data and CRC bit
		  Temp_byte2 = Read_DS18B20();
		  //Creating 16bit value with byte-shifting
		  Temp = (Temp_byte2 << 8) | Temp_byte1;
		  //Convert value from uint16_t to 12 bit value of float type
		  Temperatures[j] = (float)Temp/16;
		}
	  __HAL_ADC_ENABLE_IT(&hadc1, ADC_IT_OVR);
	  sprintf(buffer, "%.2f " ,Temperatures[0]);
	  strcat(msg, buffer);
	  sprintf(buffer, "%.2f",Temperatures[1]);
	  strcat(msg, buffer);
	  HAL_ADC_Start(&hadc1);
//************************************************BLUETOOTH COMMUNICATION*********************************************************************
	  UART1_IRQHandler();
	  sprintf(T_msg,"Buffer: %d",buffer_index);

//************************************************REAL-TIME CLOCK*******************************************************************************
	  //Get current time
	  get_time();
	  sprintf(buffer, "%s ", (char*)time);
	  //strcat(msg,buffer);
	  sprintf(buffer, "%s ", (char*)date);
	  //strcat(msg,buffer);
//************************************************UART TRAMSMIT*******************************************************************************
	  strcat(msg, "\r\n");
	  //Transmit to PC serial port
	  HAL_UART_Transmit(&huart2, (char*)msg,strlen((char*)msg), HAL_MAX_DELAY);
	  //Transmit to Bluetooth module
	  HAL_UART_Transmit(&huart1, (char*)msg,strlen((char*)msg), HAL_MAX_DELAY);
	  //Irrigation algorithm
	  if (moisture < 40)
		  irrigation = 1;
	  if (moisture > 70)
	  	  irrigation = 0;
	  //If conditions met start irrigation
	  if(__HAL_TIM_GET_COUNTER(&htim13) > 55000 && irrigation == 1)
	  {
		  __HAL_TIM_SET_COUNTER(&htim13, 0);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1 );
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1 );
		  state = 1;
	  }
	  //If conditions met stop irrigation
	  if(state == 1 && __HAL_TIM_GET_COUNTER(&htim13) > 5000 )
	  {
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0 );
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0 );
		  state = 0;
	  }
	  while((__HAL_TIM_GET_COUNTER(&htim11)) < 999);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
