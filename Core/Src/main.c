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
#include "i2c.h"
#include "usb_device.h"
#include "gpio.h"



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// HW & SW Revisions
char HW_REV[] = "HW: V1.0.0\t";
char SW_REV[] = "SW: V0.1.1\t";

void printHelp();
#define DEBUGLED
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
	return_Command = 0;							// Set return command to 0
	//  int i = 0; 									// General counter

	memset(incomig,0,sizeof(incomig));


	//  for (i = 0 ; i < sizeof(incomig) ; i++) {
	//	  incomig[i] = 0xFF;
	//  }
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(1500);
  char testDataToSend[] = "\r\n\e[2J\e[44m###### TEL-GPIO #######\e[40m\r\n";


	// Test Initial GPIO
	// Power ON GPIO
	HAL_GPIO_WritePin(GPIOB, LED1_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, LED2_Pin, 0);
	HAL_Delay(500);
	// Power OFF GPIO
	HAL_GPIO_WritePin(GPIOB, LED1_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, LED2_Pin, 1);
//	HAL_Delay(500);
//	HAL_GPIO_WritePin(GPIOB, LED2_Pin, 0);
//	HAL_GPIO_WritePin(GPIOB, LED1_Pin, 0);

	HAL_GPIO_WritePin(GPIOA, GPIO_1_Pin, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_2_Pin, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_3_Pin, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_4_Pin, 1);

// Initial data to send to the terminal 
	CDC_Transmit_FS(testDataToSend, sizeof(testDataToSend));
	HAL_Delay(10);
	CDC_Transmit_FS(HW_REV, sizeof(HW_REV));
	HAL_Delay(10);
	CDC_Transmit_FS(SW_REV, sizeof(SW_REV));
	HAL_Delay(10);
	char newLine[] = "\r\n";
	CDC_Transmit_FS(newLine, sizeof(newLine));				// Transmit new line

  
	char testFailure[] = "\e[71;\"\"P\n\r##### FAILURE ######\n\r";
	char sendASK[]  = "LEDT\r\n";
//
	// TODO Add EEPROM read function to read GPIO old GPIO values
	// TODO Set Initial GPIO Value (From EEPROM or default?)


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

//		HAL_Delay(1020);
		if (return_Command == 1) {
			#ifdef DEBUGLED
				HAL_GPIO_TogglePin(GPIOB, LED2_Pin);					// Toggle Blue LED for debugging
			#endif //end if debug led

//			char Entered_Command[] = "\033[HCommand: \r\n";
			char Entered_Command[] = "Command: \r\n";

//			CDC_Transmit_FS(Entered_Command,sizeof(Entered_Command));
//			CDC_Transmit_FS(Entered_Command,sizeof(Entered_Command));
			printHelp();
			char *ret;

			ret = strchr(incomig,'#');
			// Initialize incoming array
			if (ret >0 ) {
				HAL_GPIO_TogglePin(GPIOB, LED1_Pin);
				CDC_Transmit_FS(*ret,1);

			}


			return_Command = 0;									// Initialize Return Command to zero
			memset(incomig,0,sizeof(incomig));
			// TODO add parser for the incoming data to find the needed commands.
		} // close if for Check return string

	
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
#ifdef DEBUGLED
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1); // RED LED in case of error, only in debug mode
	char fail[] = "\a\n\r##### FAILURE ######\n\r";
	CDC_Transmit_FS(fail,sizeof(fail));
#endif
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


// Print Help screen with all available command and their implimintation.
void printHelp() {


	char printout = "\r\nPrint Help screen \r\n";
	CDC_Transmit_FS(printout,sizeof(printout));

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
