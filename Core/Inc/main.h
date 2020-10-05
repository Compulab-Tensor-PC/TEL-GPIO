/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

// General configurations 
#define DEBUGLED   // Enables various LEDS debug points
#define DEBUG     // Enable debug messages output 
// #define TESTBOARD   // Enable test board configuration


#define MAX_GPIO 					20		// Max Connected GPIO pins to board
#define INCOMING_BUFFER				128		// Incoming buffer size

// ############ GPIO function Defines #####################################

#define IN                    		0
#define OUT                   		1

#define NOT_CONNECTED         		3



// Error code return, each error should have his own code and definition
#define ERROR_01              		21     	// Error from get GPIO state function
#define ERROR_02					22		// Error code from ParseCommand function (Unknown command)


// ############ Commands Define ###########################################

#define COMMAND_SET_OUTPUT			1		// Set GPIO to Output
#define COMMAND_SET_HIGH			2 		// Set GPIO Level High on Output
#define COMMAND_SET_LOW				3 		// Set GPIO Level Low on Output
#define COMMAND_SET_INPUT 			4 		// SET GPIO Input

#define COMMAND_GET_LEVEL			5 		// Get GPIO Input value
#define COMMAND_GET_STATE			6 		// Get GPIO state


#define COMMAND_PRINT_HELP			11		// Print Help Information
#define COMMAND_DETECT_CONNECTED	12		// Detect Connected GPIO
#define COMMAND_TOGGLE_ECHO			13		// Toggle Echo to user over USB Serial




char incomig[INCOMING_BUFFER];

// ############ Commands Serial Strings ###########################################

#define HELP_COMMAND				"-H"
#define TOGGLE_ECHO					"!"
#define SET_GPIO_HIGH				"^"
#define SET_GPIO_LOW				"_"
#define SET_GPIO_INPUT				"%"
#define SET_GPIO_OUTPUT				"&"
#define GET_GPIO_STATE				"?"
#define GET_GPIO_LEVEL				"@"


uint8_t return_Command;

// Enable Echo to serial global setting
int ECHO_ENABLE;

//uint8_t MAIN_COMMAND = 0;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

int ParseCommand(char*);


/* USER CODE BEGIN EFP */
void Error_Handler(void);

// Print to USB serial
void printHelp();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO_1_Pin GPIO_PIN_0
#define GPIO_1_GPIO_Port GPIOA
#define GPIO_2_Pin GPIO_PIN_1
#define GPIO_2_GPIO_Port GPIOA
#define GPIO_3_Pin GPIO_PIN_2
#define GPIO_3_GPIO_Port GPIOA
#define GPIO_4_Pin GPIO_PIN_3
#define GPIO_4_GPIO_Port GPIOA
#define GPIO_5_Pin GPIO_PIN_4
#define GPIO_5_GPIO_Port GPIOA
#define GPIO_6_Pin GPIO_PIN_5
#define GPIO_6_GPIO_Port GPIOA
#define GPIO_7_Pin GPIO_PIN_6
#define GPIO_7_GPIO_Port GPIOA
#define GPIO_8_Pin GPIO_PIN_7
#define GPIO_8_GPIO_Port GPIOA
#define GPIO_13_Pin GPIO_PIN_0
#define GPIO_13_GPIO_Port GPIOB
#define GPIO_14_Pin GPIO_PIN_1
#define GPIO_14_GPIO_Port GPIOB
#define GPIO_15_Pin GPIO_PIN_2
#define GPIO_15_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOB
#define GPIO_9_Pin GPIO_PIN_8
#define GPIO_9_GPIO_Port GPIOA
#define GPIO_10_Pin GPIO_PIN_9
#define GPIO_10_GPIO_Port GPIOA
#define GPIO_11_Pin GPIO_PIN_10
#define GPIO_11_GPIO_Port GPIOA
#define GPIO_12_Pin GPIO_PIN_15
#define GPIO_12_GPIO_Port GPIOA
#define GPIO_16_Pin GPIO_PIN_3
#define GPIO_16_GPIO_Port GPIOB
#define GPIO_17_Pin GPIO_PIN_4
#define GPIO_17_GPIO_Port GPIOB
#define GPIO_18_Pin GPIO_PIN_5
#define GPIO_18_GPIO_Port GPIOB
#define GPIO_19_Pin GPIO_PIN_6
#define GPIO_19_GPIO_Port GPIOB
#define GPIO_20_Pin GPIO_PIN_7
#define GPIO_20_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
