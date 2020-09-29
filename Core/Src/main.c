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
#include "String.h"
#include "stdio.h"

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
char SW_REV[] = "SW: V0.1.6\t";

char InitialHeader[] = "\e[2J\e[44m###### TEL-GPIO #######\e[40m\r\n";

void printHelp();
int set_gpio(uint8_t*);
uint8_t get_gpio_state(uint8_t*);

int getGPIO(uint8_t*);


#define DEBUGLED


// Write to console function
// will search for new line string and calculate the print size.
void write( char *ptr)
{
	int len = 0;		// Create int for length
	char *pterc = ptr;	// copy pointer so can be counted safely

	// Count the array until reached new line (RETURN)
	for (len = 1 ; *pterc != '\n' ; len++ ) {
		*pterc++;
	}


	HAL_Delay(10);
	// Transmit the text
	CDC_Transmit_FS( ptr, len);

}

int io_putchar(int ch)
{
	HAL_Delay(1); // Resolves problems with print
	CDC_Transmit_FS( (uint8_t *)&ch, 1);

	return ch;
}

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
	//	  int i = 0; 									// General counter

	memset(incomig,0,sizeof(incomig));



	//write("Test",20);
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
	HAL_Delay(500);



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
	CDC_Transmit_FS(InitialHeader, sizeof(InitialHeader));
	HAL_Delay(10);
	CDC_Transmit_FS(HW_REV, sizeof(HW_REV));
	HAL_Delay(10);
	CDC_Transmit_FS(SW_REV, sizeof(SW_REV));
	HAL_Delay(10);
	char newLine[] = "\r\n";
	CDC_Transmit_FS(newLine, sizeof(newLine));				// Transmit new line

	// Command defines
	char help_command[] 			= "-H";					// Print Help screen Command
//	char set_output_dir_cmd[]			= "#";					// Set GPIO Output direction
//	char get_input_cmd[]			= "@";					// Get GPIO direction
//	char set_output_cmd[]			= "&";					// Set GPIO Input or Output direction

	char set_output_cmd[]			= "&";					// Set GPIO to Output Command
	char set_input_cmd[]			= "%";					// Set GPIO to Input Command
	char set_output_dir_cmd[]		= "#";					// Set GPIO Output direction
	char get_input_cmd[]			= "@";					// Get GPIO Input direction




	//	char testFailure[] = "\e[71;\"\"P\n\r##### FAILURE ######\n\r";
	//	char sendASK[]  = "LEDT\r\n";
	char resetScreen[] = "\ec";  							// Return screen to initial state
	int funcReturn = 0;
	//
	// TODO Add EEPROM read function to read GPIO old GPIO values
	// TODO Set Initial GPIO Value (From EEPROM or default?)
	// TODO add array with error codes that will check user input and issue correct error,
	// TODO add some memory location to store all device constants


	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		//		// Check each time the array for return string
		//		// When found start checking the array for meaningful commands
		if (strstr(incomig,"\r") != NULL ) {
			write("\r\n");		// If pressed Enter Send new line to terminal
			// Check if help command recived and print help screen if so.
			if (strstr(incomig,help_command) != NULL) {
				printHelp();
			} // Close if for print help function

			// Check for set GPIO To Output
			// ### Set GPIO OUTPUT DIRECTION ###
			else if (strstr(incomig,set_output_dir_cmd) != NULL) {
				funcReturn = set_gpio(strstr(incomig,set_output_dir_cmd));

				char setGPIO_string[2];
				itoa (funcReturn,setGPIO_string,10); // Convert from int to char

				char setGPIO[20] = "Set GPIO #:   \r\n";

				setGPIO[12] =  setGPIO_string[0]; // Place in the correct array location
				setGPIO[13] =  setGPIO_string[1];
				//				strstr(setGPIO,funcReturn);

				write(setGPIO);			// Print

			} // Close if for Set GPIO to Output


			// Get GPIO 1,0 - Note that GPIO needed to be configured to input for correct result.
			// Otherwise the result will be what programmed in setGPIO
			// ### GET GPIO INPUT STATE ###
			else if ((strstr(incomig,get_input_cmd) != NULL)) {
				//				char *test;
				char getGPIO_string[2];

				funcReturn = get_gpio_state(strstr(incomig,get_input_cmd));

				itoa (funcReturn,getGPIO_string,10); // Convert from int to char
				char getGPIO[20] = "get GPIO #:   \r\n";

				getGPIO[12] =  getGPIO_string[0]; // Place in the correct array location
				getGPIO[13] =  getGPIO_string[1];
				//				strstr(setGPIO,funcReturn);

				write(getGPIO);

			} // Close if for getGPIO command

			// set_output_cmd
			// ### SET GPIO TO OUTPUT ###
			else if ((strstr(incomig,set_output_cmd) != NULL)) {
				write("Set GPIO To OUTPUT\r\n");

				char getGPIO_string[2];

				funcReturn = get_gpio_state(strstr(incomig,get_input_cmd));
				char getGPIOIO[20] = "set GPIO #:   \r\n";
				getGPIOIO[12] =  getGPIO_string[0]; // Place in the correct array location
				getGPIOIO[13] =  getGPIO_string[1];
				write(getGPIOIO);
			}	// Close set GPIO to OUTPUT

			// Set GPIO to Input
			// ### SET GPIO TO INPUT ###
			else if ((strstr(incomig,set_input_cmd) != NULL)) {
				write("Set GPIO to Input\r\n");

			} // Close set GPIO to Input
			memset(incomig,NULL,sizeof(incomig));	// set the incoming array to zero
		}


		//		/* USER CODE END WHILE */
		//
		//		/* USER CODE BEGIN 3 */

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
	HAL_GPIO_WritePin(LED1_Pin, LED1_GPIO_Port, 1); // RED LED in case of error, only in debug mode
	HAL_GPIO_WritePin(LED2_Pin, LED2_GPIO_Port, 1);
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
	// TODO Add Terminal dynamic title change based on GPIO input??
	char resetScreen[] = "\ec";  // Return screen to initial state
	//char escape[] = "\e]0;<TEL-GPIO>\x07";
	char escape[] = "\e]0;<TEL-GPIO>\x07";
	char printout[] = "\r\n\n################ HELP ######################### \r\n";

	CDC_Transmit_FS(resetScreen, sizeof(resetScreen));
	HAL_Delay(100);
	CDC_Transmit_FS(escape, sizeof(escape));
	HAL_Delay(10);
	CDC_Transmit_FS(InitialHeader, sizeof(InitialHeader));
	HAL_Delay(10);
	CDC_Transmit_FS(HW_REV, sizeof(HW_REV));
	HAL_Delay(10);
	CDC_Transmit_FS(SW_REV,sizeof(SW_REV));
	HAL_Delay(10);
	CDC_Transmit_FS(printout,sizeof(printout));
	write("& - Set GPIO As Output Pin: \t\tEXAMPLE: &12 - Set GPIO # 12 To Output \r\n");
	write("% - Set GPIO As Input Pin:  \t\tEXANPLE: %10 - Will Set GPIO # 10 to Input\r\n ");
	write("# - Set GPIO Output Direction on Pin: \tEXAMPLE: #12,1 - Set GPIO # 12 To HIGH \r\n");
	write("@ - Get GPIO Input Value \t\tEXANPLE: @10 - Will return GPIO #10 High or Low\r\n ");


}

// If increase or decrease in GPIO numbers needed, Change the Error check for GPIO number max minimum
// And switch case statement


int set_gpio(uint8_t *set_gpioP) {
	//	uint8_t *state;
	uint8_t *gpio_num;

	int gpio_number;
	int state;

	// - Get GPIO Number and validate
	// - Get command state and validate
	// - Set the correct GPIO according to command state


	gpio_number = get_gpio(set_gpioP+1);

	// Received error from get GPIO function
	if (gpio_number == 0  ) {
		char getGpio_error[] = "Error in getGPIO\r\n";
		CDC_Transmit_FS(getGpio_error,sizeof(getGpio_error));
	}
	// Check for max minimum GPOI numbers
	else if ((gpio_number > 20) | (gpio_number < 0) ) {
		char getGpio_error[] = "Wrong GPIO number\r\n";
		CDC_Transmit_FS(getGpio_error,sizeof(getGpio_error));
	}


	//	int actual_gpio = 0;

	state = get_state(strstr(set_gpioP,","));

	if ((state < 0) | (state > 1)) {
		char getGpio_error[] = "Wrong State in set GPIO number\r\n";
		CDC_Transmit_FS(getGpio_error,sizeof(getGpio_error));
	}



	//	actual_gpio = (int)(*set_gpioP+1);

	// Add check for the gpio number range



	switch (gpio_number)
	{
	case 1:
		HAL_GPIO_WritePin(GPIO_1_GPIO_Port, GPIO_1_Pin, state);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIO_2_GPIO_Port, GPIO_2_Pin, state);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIO_3_GPIO_Port, GPIO_3_Pin, state);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIO_4_GPIO_Port, GPIO_4_Pin, state);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIO_5_GPIO_Port, GPIO_5_Pin, state);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIO_6_GPIO_Port, GPIO_6_Pin, state);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIO_7_GPIO_Port, GPIO_7_Pin, state);
		break;
	case 8:
		HAL_GPIO_WritePin(GPIO_8_GPIO_Port, GPIO_8_Pin, state);
		break;
	case 9:
		HAL_GPIO_WritePin(GPIO_9_GPIO_Port, GPIO_9_Pin, state);
		break;
	case 10:
		HAL_GPIO_WritePin(GPIO_10_GPIO_Port, GPIO_10_Pin, state);
		break;
	case 11:
		HAL_GPIO_WritePin(GPIO_11_GPIO_Port, GPIO_11_Pin, state);
		break;
	case 12:
		HAL_GPIO_WritePin(GPIO_12_GPIO_Port, GPIO_12_Pin, state);
		break;
	case 13:
		HAL_GPIO_WritePin(GPIO_13_GPIO_Port, GPIO_13_Pin, state);
		break;
	case 14:
		HAL_GPIO_WritePin(GPIO_14_GPIO_Port, GPIO_14_Pin, state);
		break;
	case 15:
		HAL_GPIO_WritePin(GPIO_15_GPIO_Port, GPIO_15_Pin, state);
		break;
	case 16:
		HAL_GPIO_WritePin(GPIO_16_GPIO_Port, GPIO_16_Pin, state);
		break;
	case 17:
		HAL_GPIO_WritePin(GPIO_17_GPIO_Port, GPIO_17_Pin, state);
		break;
	case 18:
		HAL_GPIO_WritePin(GPIO_18_GPIO_Port, GPIO_18_Pin, state);
		break;
	case 19:
		HAL_GPIO_WritePin(GPIO_19_GPIO_Port, GPIO_19_Pin, state);
		break;
	case 20:
		HAL_GPIO_WritePin(GPIO_20_GPIO_Port, GPIO_20_Pin, state);
		break;
	} // end switch statement


	//	if (*state < '0' | *state > '1' ) {
	//		return 2; // Error state code (Should be 0 or 1 for High or low)
	//	}

	return gpio_number;
}

int get_gpio(uint8_t *gpioP) {
	uint8_t *temp_i, *temp_n;
	int  gpio_n;									// init values needed for the function

	temp_i = gpioP;									// Get the first number
	temp_n = gpioP + 1 ;							// Get the next number in memory

	if ((*temp_n >= 48) & (*temp_n <= 57) ) {
		gpio_n = ((*temp_n - 48) + ( (*temp_i - 48) * 10 ));

		return gpio_n;
	}
	//	if (*temp_n == 44) { 							// Check for ',' in the second number
	//		gpio_n = (int)*temp_i-48;
	//		return gpio_n;								// Only one number, so return here
	//	}

	else {											// Calculate two dimension number
		gpio_n = (int)*temp_i-48;
		return gpio_n;
	}

	// Should reach this only in case of error (wrong text format entered)
	return gpio_n;

}

// Return the state number as received, evaluation for correct number best be handled in the main function
// As the state number range may change depending on the function calling
int get_state(uint8_t *stateP) {
	uint8_t *temp_s;
	int state = 0;

	temp_s = stateP+1;
	state = *temp_s -48;

	return state;
}


uint8_t get_gpio_state(uint8_t *get_gpioP) {
	//	uint8_t *gpio_num;
	uint8_t state = 0;

	int gpio_number;
	//	int state;

	gpio_number = get_gpio(get_gpioP+1);

	switch (gpio_number)
	{
	case 1:
		state = HAL_GPIO_ReadPin(GPIO_1_GPIO_Port, GPIO_1_Pin);
		break;
	case 2:
		state = HAL_GPIO_ReadPin(GPIO_2_GPIO_Port, GPIO_2_Pin);
		break;
	case 3:
		state = HAL_GPIO_ReadPin(GPIO_3_GPIO_Port, GPIO_3_Pin);
		break;
	case 4:
		state = HAL_GPIO_ReadPin(GPIO_4_GPIO_Port, GPIO_4_Pin);
		break;
	case 5:
		state = HAL_GPIO_ReadPin(GPIO_5_GPIO_Port, GPIO_5_Pin);
		break;
	case 6:
		state = HAL_GPIO_ReadPin(GPIO_6_GPIO_Port, GPIO_6_Pin);
		break;
	case 7:
		state = HAL_GPIO_ReadPin(GPIO_7_GPIO_Port, GPIO_7_Pin);
		break;
	case 8:
		state = HAL_GPIO_ReadPin(GPIO_8_GPIO_Port, GPIO_8_Pin);
		break;
	case 9:
		state = HAL_GPIO_ReadPin(GPIO_9_GPIO_Port, GPIO_9_Pin);
		break;
	case 10:
		state = HAL_GPIO_ReadPin(GPIO_10_GPIO_Port, GPIO_10_Pin);
		break;
	case 11:
		state = HAL_GPIO_ReadPin(GPIO_11_GPIO_Port, GPIO_11_Pin);
		break;
	case 12:
		state = HAL_GPIO_ReadPin(GPIO_12_GPIO_Port, GPIO_12_Pin);
		break;
	case 13:
		state = HAL_GPIO_ReadPin(GPIO_13_GPIO_Port, GPIO_13_Pin);
		break;
	case 14:
		state = HAL_GPIO_ReadPin(GPIO_14_GPIO_Port, GPIO_14_Pin);
		break;
	case 15:
		state = HAL_GPIO_ReadPin(GPIO_15_GPIO_Port, GPIO_15_Pin);
		break;
	case 16:
		state = HAL_GPIO_ReadPin(GPIO_16_GPIO_Port, GPIO_16_Pin);
		break;
	case 17:
		state = HAL_GPIO_ReadPin(GPIO_17_GPIO_Port, GPIO_17_Pin);
		break;
	case 18:
		state = HAL_GPIO_ReadPin(GPIO_18_GPIO_Port, GPIO_18_Pin);
		break;
	case 19:
		state = HAL_GPIO_ReadPin(GPIO_19_GPIO_Port, GPIO_19_Pin);
		break;
	case 20:
		state = HAL_GPIO_ReadPin(GPIO_20_GPIO_Port, GPIO_20_Pin);
		break;
	} // end switch statement

	return state;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
