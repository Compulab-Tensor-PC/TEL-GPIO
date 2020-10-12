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
#include <ctype.h>							// For toupper function

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
char SW_REV[] = "SW: V0.2.9\t";

char InitialHeader[] = "\e[2J\e[44m###### TEL-GPIO #######\e[40m\r\n";

int GPIO_DIR[MAX_GPIO];


// Store the Global GPIO state of each GPIO in the format:
// 	0 - Input
//	1 - Output
//  3 - Not Connected
int GPIO_STATE[MAX_GPIO];


int GPIO_CONNECTED[MAX_GPIO];

// ISR

static void EXTI0_1_IRQHandler_Config(void);

void updateGlobalDir();
void setGPIO_state();
void printGlobalState();
void updateGIPO_state(int,int);
//uint8_t get_gpio_level(uint8_t*);
//int getGPIO_state(uint8_t);

int testGPIO(int);

int getGPIO(uint8_t*);
uint8_t get_gpio_level_number(uint8_t);

uint8_t change_gpio_level_number(uint8_t, uint8_t);

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
	// Enable echo to USB serial console
	ECHO_ENABLE = 1;

	int command_code = 0;
	// initialize the global GPIO state
	setGPIO_state();
	/* USER CODE END 1 */
	//


	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	return_Command = 0;							// Set return command to 0
	memset(incomig,0,sizeof(incomig));			// Init Incoming memory array.
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
	HAL_Delay(500);								// General delay for pin init

	char resetScreen[] = "\ec";  							// Return screen to initial state
	int funcReturn = 0;


	//	EXTI0_1_IRQHandler_Config();   // TODO fix ISR for GPIO

	detectConnected();


	// Initial screen print
	write(InitialHeader);
	CDC_Transmit_FS(HW_REV, sizeof(HW_REV));
	HAL_Delay(10);
	CDC_Transmit_FS(SW_REV, sizeof(SW_REV));
	HAL_Delay(10);
	char newLine[] = "\r\n";
	CDC_Transmit_FS(newLine, sizeof(newLine));				// Transmit new line
	//
	// TODO Add EEPROM read function to read GPIO old GPIO values
	// TODO Set Initial GPIO Value (From EEPROM or default?)
	// TODO add array with error codes that will check user input and issue correct error,
	// TODO add some memory location to store all device constants
	// TODO Add Terminal dynamic title change based on GPIO input??
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		/*
		 * After each return value received the incoming buffer is evaluated,
		 * and searched for command value on first matched command the appropriate
		 * function is executed.
		 */
		// After Each Enter press the incoming buffer is evaluated,
		if (strstr(incomig,"\r") != NULL ) {
			int i = 0;
			char c;
			write("\r\n");		// If pressed Enter Send new line to terminal
			while(incomig[i]) {	// Convert strings to upper case for lower and upper commands parsing
				incomig[i] = toupper(incomig[i]);
				i++;
			}

			command_code = parse_command(incomig);
			int gpio = 0;

			switch (command_code)
			{
			case COMMAND_PRINT_HELP:					// Help Screen
				printHelp();
				break;
			case COMMAND_SET_HIGH:						// Set High
				funcReturn = set_gpio(strstr(incomig,SET_GPIO_HIGH),COMMAND_SET_HIGH);
				if (funcReturn == 0) {
					write("Command set High\r\n");
				}
				else {
					write("ERRO07\r\n");
				}
				break;
			case COMMAND_SET_LOW:						// Set Low
				funcReturn = set_gpio(strstr(incomig,SET_GPIO_LOW),COMMAND_SET_LOW);
				if (funcReturn == 0) {
					write("Command set High\r\n");
				}
				else {
					write("ERRO07\r\n");
				}
				break;
			case COMMAND_ENABLE_ECHO:					// Enable Echo
				toggleEcho(1);
				break;
			case COMMAND_DISABLE_ECHO:					// Disable echo
				toggleEcho(0);
				break;
			case COMMAND_SET_INPUT:						// Set INPUT
				funcReturn = set_gpio(strstr(incomig,SET_GPIO_INPUT),COMMAND_SET_INPUT);
				write("Set Input\r\n");
				break;
			case COMMAND_SET_OUTPUT:					// Set OUTPUT
				funcReturn = set_gpio(strstr(incomig,SET_GPIO_OUTPUT),COMMAND_SET_OUTPUT);
				write("Set Output\r\n");
				break;
			case COMMAND_GET_STATE:						// Get State
				funcReturn = get_gpio(strstr(incomig,GET_GPIO_STATE),COMMAND_GET_STATE);
				//				write("Get GPIO State\r\n");
				break;
			case COMMAND_GET_LEVEL:						// Get Level
				funcReturn = get_gpio(strstr(incomig,GET_GPIO_LEVEL),COMMAND_GET_LEVEL);
				//				write("Get GPIO Level\r\n");
				break;
			default:									// no command / wrong command
				write("Wrong command entered\r\n");
				break;
			}


			//			// ### HELP SCREEN ###
			//			if (strstr(incomig,help_command) != NULL) {
			//				//				printHelp();
			//			} // Close if for print help function
			//
			//			// ### SET GPIO LEVEL HIGH ###
			//			else if (strstr(incomig,set_level_high) != NULL) {
			//				//				write("SET GPIO LEVEL HIGH\r\n");
			//				funcReturn = set_gpio(strstr(incomig,set_level_high),1);
			//
			//				if (funcReturn != 0) {
			//
			//					char setGPIO_string[2];
			//					itoa (funcReturn,setGPIO_string,10); // Convert from int to char
			//
			//					char setGPIO[30] = "Set GPIO #:   to High\r\n";
			//
			//					setGPIO[11] =  setGPIO_string[0]; // Place in the correct array location
			//					setGPIO[12] =  setGPIO_string[1];
			//					//				strstr(setGPIO,funcReturn);
			//
			//					write(setGPIO);			// Print
			//				}
			//			}
			//			//set_level_low
			//			else if (strstr(incomig,set_level_low) != NULL) {
			//				//				write("SET GPIO LEVEL HIGH\r\n");
			//				funcReturn = set_gpio(strstr(incomig,set_level_low),0);
			//
			//				if (funcReturn != 0) {
			//
			//					char setGPIO_string[2];
			//					itoa (funcReturn,setGPIO_string,10); // Convert from int to char
			//
			//					char setGPIO[30] = "Set GPIO #:   to Low\r\n";
			//
			//					setGPIO[11] =  setGPIO_string[0]; // Place in the correct array location
			//					setGPIO[12] =  setGPIO_string[1];
			//					//				strstr(setGPIO,funcReturn);
			//
			//					write(setGPIO);			// Print
			//				}
			//			}
			//
			//			// ### TOGGLE ECHO ###
			//			else if (strstr(incomig,toggle_echo) != NULL) {
			//
			//				//				toggleEcho();
			//			}
			//
			//			// Get GPIO 1,0 - Note that GPIO needed to be configured to input for correct result.
			//			// Otherwise the result will be what programmed in setGPIO
			//			// ### GET GPIO INPUT STATE ###
			//			else if ((strstr(incomig,get_input_cmd) != NULL)) {
			//				//				char *test;
			//				char getGPIO_string[2];
			//
			//				funcReturn = get_gpio_level(strstr(incomig,get_input_cmd));
			//
			//				if ((funcReturn == 0) | (funcReturn == 1) ) {
			//
			//					itoa (funcReturn,getGPIO_string,10); // Convert from int to char
			//					char getGPIO[30] = "GPIO Dir#:    \r\n";
			//
			//					getGPIO[11] =  getGPIO_string[0]; // Place in the correct array location
			//					getGPIO[12] =  getGPIO_string[1];
			//					//				strstr(setGPIO,funcReturn);
			//
			//					write(getGPIO);
			//				}
			//
			//
			//			} // Close if for getGPIO command
			//			// set_output_cmd
			//			// ### SET GPIO TO OUTPUT ###
			//			else if ((strstr(incomig,set_output_cmd) != NULL)) {
			//
			//
			//				funcReturn = set_gpio_output(strstr(incomig,set_output_cmd));
			//
			//
			//				char getGPIO_string[2];
			//
			//				itoa (funcReturn,getGPIO_string,10); // Convert from int to char
			//
			//
			//				char getGPIOIO[25] = "Set Output GPIO #:   \r\n";
			//				getGPIOIO[19] =  getGPIO_string[0]; // Place in the correct array location
			//				getGPIOIO[20] =  getGPIO_string[1];
			//				write(getGPIOIO);
			//			}	// Close set GPIO to OUTPUT
			//
			//			// Set GPIO to Input
			//			// ### SET GPIO TO INPUT ###
			//			else if ((strstr(incomig,set_input_cmd) != NULL)) {
			//				//				write("Set GPIO to Input\r\n");
			//
			//				funcReturn = set_gpio_input(strstr(incomig,set_input_cmd));
			//
			//				char getGPIO_string[2];
			//
			//				itoa (funcReturn,getGPIO_string,10); // Convert from int to char
			//				char getGPIOIO[25] = "Set Input GPIO #:   \r\n";
			//				getGPIOIO[18] =  getGPIO_string[0]; // Place in the correct array location
			//				getGPIOIO[19] =  getGPIO_string[1];
			//				write(getGPIOIO);
			//
			//
			//			} // Close set GPIO to Input
			//
			//			else if ((strstr(incomig,get_gpio_state) != NULL)) {
			//				//				write("Set GPIO to Input\r\n");
			//				int gpio_number;
			//				char getstate_string[2];
			//				char getGPIOIO[40] = "GPIO state (IN / OUT):   \r\n";
			//
			//
			//				gpio_number = get_gpio(strstr(incomig,get_gpio_state)+1);
			//
			//				//				funcReturn = getGPIO_state(strstr(incomig,get_gpio_state));
			//
			//				funcReturn = getGPIO_state(gpio_number);
			//
			//				if ((funcReturn == 1) | (funcReturn == 0) ) {
			//
			//					itoa (funcReturn,getstate_string,10); // Convert from int to char
			//
			//					getGPIOIO[23] =  getstate_string[0]; 	// Place in the correct array location
			//					getGPIOIO[24] =  getstate_string[1];	// int result
			//				}
			//				else if (funcReturn == 2) {
			//					getGPIOIO[23] =  'X'; // Place in the correct array location
			//				}
			//				else if (funcReturn == 3) {
			//					getGPIOIO[23] =  '*'; // Place in the correct array location
			//				}
			//
			//				write(getGPIOIO);
			//
			//			} // Close set GPIO to Input
			memset(incomig,(char)0,sizeof(incomig));	// set the incoming array to zero
			funcReturn = 0;
			command_code = 0;
		} // Close if (if pressed Enter)
	}	// Close main while loop
}	// Close main()

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


/**
 * @brief  Print to USB serial Help informarion
 * @param  None
 * @retval None
 */
void printHelp() {
	char resetScreen[] = "\ec";  // Return screen to initial state
	//char escape[] = "\e]0;<TEL-GPIO>\x07";
	char escape[] = "\e]0;<TEL-GPIO>\x07";
	char printout[] = "\r\n\n################ HELP ############################# \r\n";

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
	write("^ - Set GPIO LEVEL HIGH '^##'\t\tEXAMPLE: '^3' - Will Set GPIO # 3 To High Level\r\n");
	write("_ - Set GPIO LEVEL LOW  '_##'\t\tEXAMPLE: '_06' - Will set GPIO # 6 To Low Level\r\n");
	write("@ - Get GPIO level Value \t\tEXANPLE: @10 - Will return GPIO #10 High or Low\r\n ");
	write("? - get GPIO state '?4\t\t\tEXAMPLE: '?3' - Will return 1- initially.\r\n");
	write("\t\t\t\t\tNot connected return X and out of bonds return *\r\n");
	write("NOTE: GPIO can be entered as 03 or 3 for the same GPIO number\r\n");
	write("################ SETTINGS ##########################\r\n");
	write("\r\n");
	write("-D - Disable USB serial Echo Output\r\n");
	write("-E - Enables USB serial Echo Output\r\n");
	write("\r\n");
	write("################ GLOBAL INFO ########################\r\n");
	write("\r\n");
	//	write("");
	updateGlobalDir();
	printGlobalState();
	printConnected();
	write("-----------------------------------------------------------------------\r\n");
}

/**
 * @brief  Return Command code of the passed incomming buffer pointer
 * @param  Pointer to Incoming buffer
 * @retval int command code
 */
int parse_command(char* incomingBuffer) {

	int return_command = 0;
	if (strstr(incomingBuffer,HELP_COMMAND) != NULL) {
		return_command = COMMAND_PRINT_HELP;
	}
	else if (strstr(incomingBuffer,SET_GPIO_HIGH) != NULL) {
		return_command = COMMAND_SET_HIGH;
	}
	else if (strstr(incomingBuffer,SET_GPIO_LOW) != NULL) {
		return_command = COMMAND_SET_LOW;
	}
	else if ((strstr(incomingBuffer,SET_GPIO_INPUT) != NULL)) {
		return_command = COMMAND_SET_INPUT;
	}
	else if ((strstr(incomingBuffer,SET_GPIO_OUTPUT) != NULL)) {
		return_command = COMMAND_SET_OUTPUT;
	}
	else if ((strstr(incomingBuffer,GET_GPIO_STATE) != NULL)) {
		return_command = COMMAND_GET_STATE;
	}
	else if ((strstr(incomingBuffer,GET_GPIO_LEVEL) != NULL)) {
		return_command = COMMAND_GET_LEVEL;
	}
	else if ((strstr(incomingBuffer,TOGGLE_ISR_GPIO) != NULL)) {
		return_command = COMMAND_TOGGLE_ISR;
	}
	else if ((strstr(incomingBuffer,DISABLE_ECHO) != NULL)) {
		return_command = COMMAND_DISABLE_ECHO;
	}
	else if ((strstr(incomingBuffer,ENABLE_ECHO) != NULL)) {
		return_command = COMMAND_ENABLE_ECHO;
	}

	return return_command;
}


/**
 * @brief  Return gpio number from the incoming buffer reference
 * @param  Pointer to Incoming buffer
 * @retval int gpio number
 */
int parse_gpio(char *gpioP) {
	char *temp_i, *temp_n;
	int  gpio_n;

	temp_i = gpioP;									// Get the first number
	temp_n = gpioP + 1 ;							// Get the next number in memory

	if ((*temp_n >= 48) & (*temp_n <= 57) ) {
		gpio_n = ((*temp_n - 48) + ( (*temp_i - 48) * 10 ));

		return gpio_n;
	} 	// Close if test for integer number test
	else {											// Calculate two dimension number
		gpio_n = (int)*temp_i-48;
		return gpio_n;
	}	// Close else

	// Should reach this only in case of error (wrong text format entered)
	return gpio_n;
}


/**
 * @brief  	Set GPIO as per command received, handles all the set gpio commands
 * 			Only GPIO range numbering is tested.
 *
 * @param  	Pointer to Incoming buffer
 * @param   int Incoming command value
 * @retval 	Return code of the operation
 */
int set_gpio(char *buffer,int command) {
	//	uint8_t *state;
	int gpio_number = 0;

	gpio_number = parse_gpio(buffer+1);

	// TEST GPIO LIMIT
	if (testGPIO(gpio_number) == 0) {

		switch (command)
		{
		case (COMMAND_SET_HIGH):{		// Check if GPIO in Output state if not return ERROR_07
			if (get_gpio_state(gpio_number) != OUT ) {
				return ERROR_07;
				break;
			}
			set_gpio_level(gpio_number,GPIO_HIGH);
			break;
		}
		case (COMMAND_SET_LOW):{		// Check if GPIO in Output state if not return ERROR_07
			if (get_gpio_state(gpio_number) != OUT ) {
				return ERROR_07;
				break;
			}
			set_gpio_level(gpio_number,GPIO_LOW);
			break;
		}
		case (COMMAND_SET_OUTPUT):{
			set_gpio_state(gpio_number,GPIO_OUTPUT);
			break;
		}
		case (COMMAND_SET_INPUT):{
			set_gpio_state(gpio_number,GPIO_INPUT);
			break;
		}
		}
		return 0;
	}	// Close If Check for GPIO limit

	else {
		write("GPIO Number out of limits\r\n");
		return ERROR_03;
	} // Close else for GPIO limit
} // Close function set GPIO


/**
 * @brief  	Get GPIO state or level as per command received handles all the get GPIO commands
 * 			from main switch case
 *
 * @param  	Pointer to Incoming buffer
 * @param   int Incoming command value
 * @retval 	Return code of the operation- GPIO desired information.
 */
int get_gpio(char *buffer, int command) {
	//	uint8_t *state;
	int gpio_number = 0;
	int gpio_state = 0;
	char write_output[4] = "";
	char getstate_string[10] = "";


	gpio_number = parse_gpio(buffer+1);

	//		itoa (gpio_number,getstate_string,10); // Convert from int to char

//	strcat(write_output, getstate_string);	//


	// TEST GPIO LIMIT
	if (testGPIO(gpio_number) == 0) {

		switch (command)
		{
		case (COMMAND_GET_LEVEL):{
			gpio_state  = get_gpio_level(gpio_number);
			//				strcat(write_output, "GPIO# ");
//			itoa (gpio_number,getstate_string,10); // Convert from int to char
//			strcat(write_output,getstate_string);
//			strcat(write_output, " level: ");
			itoa (gpio_state,getstate_string,10); // Convert from int to char
//			strcat(write_output,getstate_string);
			break;
		}
		case (COMMAND_GET_STATE):{
			gpio_state =  get_gpio_state(gpio_number);
//			itoa (gpio_number,getstate_string,10); // Convert from int to char
//			strcat(write_output,getstate_string);
//			strcat(write_output, " state: ");
			itoa (gpio_state,getstate_string,10); // Convert from int to char
//			strcat(write_output,getstate_string);
			break;
		}
		default:
			return ERROR_06;
			break;

		}
		strcat(getstate_string,"\r\n");
		write(getstate_string);

		//			itoa (gpio_state,getstate_string,10); // Convert from int to char

		//											getGPIOIO[12] =  getstate_string[0]; 	// Place in the correct array location
		//											getGPIOIO[13] =  getstate_string[1];	// int result


		//										write(getGPIOIO);
		return gpio_state;
	}	// Close If Check for GPIO limit

	else {
		write("GPIO Number out of limits\r\n");
		return ERROR_03;
	} // Close else for GPIO limit

	//	return gpio_number;
}


/**
 * @brief Set GPIO state( output / input)
 *
 *
 * @param  	int GPIO number in Integer
 * @param	int GPIO level in integer 1 -Output 0 - Input
 * @retval Return the changed to OUTPUT GPIO, will return 0 on ERROR. TODO add ERROR code
 */
int set_gpio_state(int gpioNumber, int state) {

	// Validate GPIO range
	if (testGPIO(gpioNumber) == 0) {

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		// Update GPIO state to the GPIO state array
		switch (state) {
		case 0:    										// Input
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			//			GPIO_InitStruct.Pull = GPIO_PULLDOWN;
			//			write("State INPUT\r\n");
			updateGIPO_state(gpioNumber,0);				// Update global array for GPIO state

			break;
		case 1: 										// Output
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			updateGIPO_state(gpioNumber,1);				// Update global array for GPIO state
			break;
		default:
			write("State wrong\r\n");
			return 0;
		}

		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

		switch (gpioNumber)
		{
		case 1:
			GPIO_InitStruct.Pin = GPIO_1_Pin;
			HAL_GPIO_Init(GPIO_1_GPIO_Port, &GPIO_InitStruct);
			break;
		case 2:
			GPIO_InitStruct.Pin = GPIO_2_Pin;
			HAL_GPIO_Init(GPIO_2_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 3:
			GPIO_InitStruct.Pin = GPIO_3_Pin;
			HAL_GPIO_Init(GPIO_3_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 4:
			GPIO_InitStruct.Pin = GPIO_4_Pin;
			HAL_GPIO_Init(GPIO_4_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 5:
			GPIO_InitStruct.Pin = GPIO_5_Pin;
			HAL_GPIO_Init(GPIO_5_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 6:
			GPIO_InitStruct.Pin = GPIO_6_Pin;
			HAL_GPIO_Init(GPIO_6_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 7:
			GPIO_InitStruct.Pin = GPIO_7_Pin;
			HAL_GPIO_Init(GPIO_7_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 8:
			GPIO_InitStruct.Pin = GPIO_8_Pin;
			HAL_GPIO_Init(GPIO_8_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 9:
			GPIO_InitStruct.Pin = GPIO_9_Pin;
			HAL_GPIO_Init(GPIO_9_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 10:
			GPIO_InitStruct.Pin = GPIO_10_Pin;
			HAL_GPIO_Init(GPIO_10_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 11:
			GPIO_InitStruct.Pin = GPIO_11_Pin;
			HAL_GPIO_Init(GPIO_11_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 12:
			GPIO_InitStruct.Pin = GPIO_12_Pin;
			HAL_GPIO_Init(GPIO_12_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 13:
			GPIO_InitStruct.Pin = GPIO_13_Pin;
			HAL_GPIO_Init(GPIO_13_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 14:
			GPIO_InitStruct.Pin = GPIO_14_Pin;
			HAL_GPIO_Init(GPIO_14_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 15:
			GPIO_InitStruct.Pin = GPIO_15_Pin;
			HAL_GPIO_Init(GPIO_15_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 16:
			GPIO_InitStruct.Pin = GPIO_16_Pin;
			HAL_GPIO_Init(GPIO_16_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 17:
			GPIO_InitStruct.Pin = GPIO_17_Pin;
			HAL_GPIO_Init(GPIO_17_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 18:
			GPIO_InitStruct.Pin = GPIO_18_Pin;
			HAL_GPIO_Init(GPIO_18_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 19:
			GPIO_InitStruct.Pin = GPIO_19_Pin;
			HAL_GPIO_Init(GPIO_19_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 20:
			GPIO_InitStruct.Pin = GPIO_20_Pin;
			HAL_GPIO_Init(GPIO_20_GPIO_Port,  &GPIO_InitStruct);
			break;
		}
	}
	else {
		write("Set GPIO Input Number wrong\r\n ");
	}
	return gpioNumber;
}


/**
 * @brief Set GPIO to the received level state
 *
 *
 *
 * @param  	GPIO number in Integer
 * @param	GPIO level in integer 1 for High 0 for Low
 * @retval Return the changed to OUTPUT GPIO, will return 0 on ERROR.
 */
int set_gpio_level(int gpioNum, int level) {

	if ((level == 0) | (level==1) ) {

		switch (gpioNum)
		{
		case 1:
			HAL_GPIO_WritePin(GPIO_1_GPIO_Port, GPIO_1_Pin, level);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIO_2_GPIO_Port, GPIO_2_Pin, level);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIO_3_GPIO_Port, GPIO_3_Pin, level);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIO_4_GPIO_Port, GPIO_4_Pin, level);
			break;
		case 5:
			HAL_GPIO_WritePin(GPIO_5_GPIO_Port, GPIO_5_Pin, level);
			break;
		case 6:
			HAL_GPIO_WritePin(GPIO_6_GPIO_Port, GPIO_6_Pin, level);
			break;
		case 7:
			HAL_GPIO_WritePin(GPIO_7_GPIO_Port, GPIO_7_Pin, level);
			break;
		case 8:
			HAL_GPIO_WritePin(GPIO_8_GPIO_Port, GPIO_8_Pin, level);
			break;
		case 9:
			HAL_GPIO_WritePin(GPIO_9_GPIO_Port, GPIO_9_Pin, level);
			break;
		case 10:
			HAL_GPIO_WritePin(GPIO_10_GPIO_Port, GPIO_10_Pin, level);
			break;
		case 11:
			HAL_GPIO_WritePin(GPIO_11_GPIO_Port, GPIO_11_Pin, level);
			break;
		case 12:
			HAL_GPIO_WritePin(GPIO_12_GPIO_Port, GPIO_12_Pin, level);
			break;
		case 13:
			HAL_GPIO_WritePin(GPIO_13_GPIO_Port, GPIO_13_Pin, level);
			break;
		case 14:
			HAL_GPIO_WritePin(GPIO_14_GPIO_Port, GPIO_14_Pin, level);
			break;
		case 15:
			HAL_GPIO_WritePin(GPIO_15_GPIO_Port, GPIO_15_Pin, level);
			break;
		case 16:
			HAL_GPIO_WritePin(GPIO_16_GPIO_Port, GPIO_16_Pin, level);
			break;
		case 17:
			HAL_GPIO_WritePin(GPIO_17_GPIO_Port, GPIO_17_Pin, level);
			break;
		case 18:
			HAL_GPIO_WritePin(GPIO_18_GPIO_Port, GPIO_18_Pin, level);
			break;
		case 19:
			HAL_GPIO_WritePin(GPIO_19_GPIO_Port, GPIO_19_Pin, level);
			break;
		case 20:
			HAL_GPIO_WritePin(GPIO_20_GPIO_Port, GPIO_20_Pin, level);
			break;
		default:
		{
			// Will print this error only if debug enabled.
#ifdef DEBUG
			write("GPIO number Out of bounds\r\n");
#endif //end if debug led
			return 0;
		}
		} // end switch statement
	} // Close if check for level number
	else {
		// Will print this error only if debug enabled.
#ifdef DEBUG
		write("level out of bounds error\r\n");
#endif //end if debug led
		return 0;
	}
	return gpioNum;
}


//// parse incoming buffer and  return the GPIO integer value
//int return_gpio(uint8_t *gpioP) {
//	uint8_t *temp_i, *temp_n;
//	int  gpio_n;									// init values needed for the function
//
//	temp_i = gpioP;									// Get the first number
//	temp_n = gpioP + 1 ;							// Get the next number in memory
//
//	if ((*temp_n >= 48) & (*temp_n <= 57) ) {
//		gpio_n = ((*temp_n - 48) + ( (*temp_i - 48) * 10 ));
//
//		return gpio_n;
//	}
//	//	if (*temp_n == 44) { 							// Check for ',' in the second number
//	//		gpio_n = (int)*temp_i-48;
//	//		return gpio_n;								// Only one number, so return here
//	//	}
//
//	else {											// Calculate two dimension number
//		gpio_n = (int)*temp_i-48;
//		return gpio_n;
//	}
//
//	// Should reach this only in case of error (wrong text format entered)
//	return gpio_n;
//
//}


// Perform test on the received GPIO, for now only the max and min GPIO number
/**
 * @brief  Receive and test GPIO number
 *
 *
 *        @note
 *        This function test's GPIO number, for now (09/20) only maximum
 *        and minimum number range, later maybe some conditional test will be needed.
 *        return 0 on OK and 1 on error
 *
 * @param  GPIO number to be tested
 * @retval Test result 0 on OK evrething else error.
 */
int testGPIO(int GPIO_NUM) {

	// Check if GPIO is in correct range
	if ((GPIO_NUM > 0) & (GPIO_NUM <= MAX_GPIO)) {
		return 0;
	}
	else
		return 1;
}



//// Return the state number as received, evaluation for correct number best be handled in the main function
//// As the state number range may change depending on the function calling
//int get_state(uint8_t *stateP) {
//	uint8_t *temp_s;
//	int state = 0;
//
//	temp_s = stateP+1;
//	state = *temp_s -48;
//
//	return state;
//}


/**
 * @brief Get the GPIO level state
 *
 *
 * @param  	Pointer to GPIO number
 * @retval 	Return GPIO level state 0 - for low 1 - for High 2 - for ERROR
// */
//uint8_t get_gpio_level(uint8_t *get_gpioP) {
//	//	uint8_t *gpio_num;
//	uint8_t state = 0;
//
//	int gpio_number;
//	//	int state;
//
//	gpio_number = return_gpio(get_gpioP+1);
//
//	// Test GPIO limits
//	if (testGPIO(gpio_number) == 0) {
//
//		state = get_gpio_level_number(gpio_number);
//
//		return state;
//
//	}
//
//	else {
//		write("Get GPIO stat GPIO # out of limits\r\n");
//		return 2;
//	}
//}


/**
 * @brief Get the GPIO level
 *
 *
 * @param  	Integer GPIO number
 * @retval 	Return GPIO level state 0 - for low 1 - for High 3 - for ERROR
 */
int get_gpio_level(int gpioNumber) {


	int state;

	switch (gpioNumber)
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
	default:
		return ERROR_03; // Error state
	} // end switch statement
	return state;
}

// Return the passed GPIO number state (INPUT / OUTPUT)
// 0 - Input
// 1 - Output
// 2 - Not connected
// 3 - ERROR
int get_gpio_state(int gpio) {
	int GPIO_state = 0;

	if (testGPIO(gpio) == 0) {
		GPIO_state = GPIO_STATE[gpio-1];
	}

	else
		GPIO_state = ERROR_03;		// Error return code

	return GPIO_state;
}

/**
 * @brief  Invoked after parsing incoming data buffer for set GPIO to OUTPUT command
 *
 *
 *        @note
 *        This function should return GPIO number that is changed to OUTPUT state
 *        and return 0 if something went wrong:
 *        - Wrong GPIO number (above or below limit)
 *        - Wrong string format (char instead of GPIO number after command)
 *        Correct or incorrect GPIO tested by testGPIO function.
 *
 * @param  set_gpio_out: Pointer to parsed command char
 * @retval Return the changed to OUTPUT GPIO, will return 0 on ERROR.
 */
int set_gpio_output(int set_gpio_out, int parameter) {

	int gpio_number;

	gpio_number = return_gpio(set_gpio_out+1);		// Get GPIO number

	if (testGPIO(gpio_number) == 0) {			// Test if GPIO is in range
		return change_gpio_level_number(gpio_number,1);
	}
	else
		return 0;								// Failed test in range return Error
}


/**
 * @brief  Change GPIO to Input state
 *
 *
 *        @note
 *        Set GPIO input with optional additional parameters of the Input:
 *        IN_PU	- Input with pull up	resistor
 *        IN_PD	- Input with pull down	resistor
 *		  IN_Z	- Input without any resistor
 *
 * @param  set_gpio_out: Pointer to parsed command char
 * @retval Return the changed to INPUT GPIO, will return 0 on ERROR.
 */
int set_gpio_input(int set_gpio_in, int parameter) {

	int returnCode = 0;
	if (testGPIO(set_gpio_in) != 0) {			// Test if GPIO is in range and break here if so
		returnCode = ERROR_03;
		return returnCode;
	}

	// Init GPIO struct
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	switch (parameter)
	{
	case IN_PU: {
		GPIO_InitStruct.Pull = GPIO_PULLUP;

		break;
	}
	case IN_PD: {
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;

		break;
	}
	case IN_Z: {
		GPIO_InitStruct.Pull = GPIO_NOPULL;

	}
	default:
		returnCode = ERROR_05;
		return returnCode;
	}


	switch (set_gpio_in)
	{
	case 1:
		GPIO_InitStruct.Pin = GPIO_1_Pin;
		HAL_GPIO_Init(GPIO_1_GPIO_Port, &GPIO_InitStruct);
		break;
	case 2:
		GPIO_InitStruct.Pin = GPIO_2_Pin;
		HAL_GPIO_Init(GPIO_2_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 3:
		GPIO_InitStruct.Pin = GPIO_3_Pin;
		HAL_GPIO_Init(GPIO_3_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 4:
		GPIO_InitStruct.Pin = GPIO_4_Pin;
		HAL_GPIO_Init(GPIO_4_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 5:
		GPIO_InitStruct.Pin = GPIO_5_Pin;
		HAL_GPIO_Init(GPIO_5_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 6:
		GPIO_InitStruct.Pin = GPIO_6_Pin;
		HAL_GPIO_Init(GPIO_6_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 7:
		GPIO_InitStruct.Pin = GPIO_7_Pin;
		HAL_GPIO_Init(GPIO_7_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 8:
		GPIO_InitStruct.Pin = GPIO_8_Pin;
		HAL_GPIO_Init(GPIO_8_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 9:
		GPIO_InitStruct.Pin = GPIO_9_Pin;
		HAL_GPIO_Init(GPIO_9_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 10:
		GPIO_InitStruct.Pin = GPIO_10_Pin;
		HAL_GPIO_Init(GPIO_10_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 11:
		GPIO_InitStruct.Pin = GPIO_11_Pin;
		HAL_GPIO_Init(GPIO_11_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 12:
		GPIO_InitStruct.Pin = GPIO_12_Pin;
		HAL_GPIO_Init(GPIO_12_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 13:
		GPIO_InitStruct.Pin = GPIO_13_Pin;
		HAL_GPIO_Init(GPIO_13_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 14:
		GPIO_InitStruct.Pin = GPIO_14_Pin;
		HAL_GPIO_Init(GPIO_14_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 15:
		GPIO_InitStruct.Pin = GPIO_15_Pin;
		HAL_GPIO_Init(GPIO_15_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 16:
		GPIO_InitStruct.Pin = GPIO_16_Pin;
		HAL_GPIO_Init(GPIO_16_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 17:
		GPIO_InitStruct.Pin = GPIO_17_Pin;
		HAL_GPIO_Init(GPIO_17_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 18:
		GPIO_InitStruct.Pin = GPIO_18_Pin;
		HAL_GPIO_Init(GPIO_18_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 19:
		GPIO_InitStruct.Pin = GPIO_19_Pin;
		HAL_GPIO_Init(GPIO_19_GPIO_Port,  &GPIO_InitStruct);
		break;
	case 20:
		GPIO_InitStruct.Pin = GPIO_20_Pin;
		HAL_GPIO_Init(GPIO_20_GPIO_Port,  &GPIO_InitStruct);
		break;
	}

	return returnCode;
}




//
////	gpio_number = get_gpio(set_gpio_in+1);		// Get GPIO number
//
//	if (testGPIO(gpio_number) == 0) {			// Test if GPIO is in range
//		return change_gpio_level_number(gpio_number,0);
//	}
//	else
//		return returnCode;								// Failed test in range return Error









void updateGlobalDir() {

	char GPIO_INPUT_PRINT[60] = "GPIO Input Read:\t";
	char getGPIO_string[2];

	GPIO_DIR[0] = HAL_GPIO_ReadPin(GPIO_1_GPIO_Port, GPIO_1_Pin);
	GPIO_DIR[1] = HAL_GPIO_ReadPin(GPIO_2_GPIO_Port, GPIO_2_Pin);
	GPIO_DIR[2] = HAL_GPIO_ReadPin(GPIO_3_GPIO_Port, GPIO_3_Pin);
	GPIO_DIR[3] = HAL_GPIO_ReadPin(GPIO_4_GPIO_Port, GPIO_4_Pin);
	GPIO_DIR[4] = HAL_GPIO_ReadPin(GPIO_5_GPIO_Port, GPIO_5_Pin);
	GPIO_DIR[5] = HAL_GPIO_ReadPin(GPIO_6_GPIO_Port, GPIO_6_Pin);
	GPIO_DIR[6] = HAL_GPIO_ReadPin(GPIO_7_GPIO_Port, GPIO_7_Pin);
	GPIO_DIR[7] = HAL_GPIO_ReadPin(GPIO_8_GPIO_Port, GPIO_8_Pin);
	GPIO_DIR[8] = HAL_GPIO_ReadPin(GPIO_9_GPIO_Port, GPIO_9_Pin);
	GPIO_DIR[9] = HAL_GPIO_ReadPin(GPIO_10_GPIO_Port, GPIO_10_Pin);
	GPIO_DIR[10] = HAL_GPIO_ReadPin(GPIO_11_GPIO_Port, GPIO_11_Pin);
	GPIO_DIR[11] = HAL_GPIO_ReadPin(GPIO_12_GPIO_Port, GPIO_12_Pin);
	GPIO_DIR[12] = HAL_GPIO_ReadPin(GPIO_13_GPIO_Port, GPIO_13_Pin);
	GPIO_DIR[13] = HAL_GPIO_ReadPin(GPIO_14_GPIO_Port, GPIO_14_Pin);
	GPIO_DIR[14] = HAL_GPIO_ReadPin(GPIO_15_GPIO_Port, GPIO_15_Pin);
	GPIO_DIR[15] = HAL_GPIO_ReadPin(GPIO_16_GPIO_Port, GPIO_16_Pin);
	GPIO_DIR[16] = HAL_GPIO_ReadPin(GPIO_17_GPIO_Port, GPIO_17_Pin);
	GPIO_DIR[17] = HAL_GPIO_ReadPin(GPIO_18_GPIO_Port, GPIO_18_Pin);
	GPIO_DIR[18] = HAL_GPIO_ReadPin(GPIO_19_GPIO_Port, GPIO_19_Pin);
	GPIO_DIR[19] = HAL_GPIO_ReadPin(GPIO_20_GPIO_Port, GPIO_20_Pin);


	for (int n = 0 ; n < MAX_GPIO ; n++) {

		itoa (GPIO_DIR[n],getGPIO_string,10); // Convert from int to char
		strcat(GPIO_INPUT_PRINT,getGPIO_string);
		if (n < MAX_GPIO-1)
			strcat(GPIO_INPUT_PRINT,",");

	}

	//	strcat(GPIO_INPUT_PRINT, ((char)GPIO_DIR[0])-48);


	strcat(GPIO_INPUT_PRINT, "\r\n");
	write(GPIO_INPUT_PRINT);
}


/**
 * @brief  Print the global State of Each GPIO pin
 *
 *
 *        @note
 *        Print from the global array GPIO_STATE
 *        The printout is in the following format:
 *        	1 - GPIO is in Output state
 *        	2 - GPIO is in Input state
 *        	x - Not Connected
 */
void printGlobalState() {

	char GPIO_STATE_PRINT[70] = "GPIO State (IN / OUT):\t";


	for (int n = 0 ; n < MAX_GPIO ; n++) {

		switch (GPIO_STATE[n])
		{

		case 0: // Input Pin case
			strcat(GPIO_STATE_PRINT,"0");
			break;
		case 1:	// Output pin Case
			strcat(GPIO_STATE_PRINT,"1");
			break;
		case 3:	// Not Connected pin case
			strcat(GPIO_STATE_PRINT,"X");
			break;
		default:	// Error pin case
			strcat(GPIO_STATE_PRINT,"y");

		}
		if (n < MAX_GPIO-1)
			strcat(GPIO_STATE_PRINT,",");

	}

	strcat(GPIO_STATE_PRINT, "\r\n");
	write(GPIO_STATE_PRINT);

}


/**
 * @brief  Toggle echo printout to user
 * @param  None
 * @retval None
 */
void toggleEcho(int stat) {

	if (stat == 1) {
		ECHO_ENABLE = 1;
	}
	else if (stat == 0 ) {
		ECHO_ENABLE = 0;
	}



}


/**
 * @brief  Update GPIO state array to its initial state
 * @param  None
 * @retval None
 */
void setGPIO_state() {
	for (int n = 0; n < MAX_GPIO; n++ ){
		GPIO_STATE[n] = 1;
	}
}


/**
 * @brief	Update GPIO state array by the given number and state
 * 			No update will happen on icorrect GPIO number.
 * 			State number is not tested
 * @param	gpio - gpio number to be updated
 * @param	state - State to be updated to 0 - In, 1 - Out
 * @retval	None
 */
void updateGIPO_state(int gpio, int state){
	// Test GPIO range and update the currect array location with the desired state
	if (testGPIO(gpio) == 0) {
		GPIO_STATE[gpio-1] = state;
	}
}



/* Check and Update the global array for connected GPIOs
 * The test should be as follows:
 * Set All GPIO's As Input PD and test all up are connected.
 * Set all GPIO's as Input PU and test all down are connected.
 */
void detectConnected() {
	// GPIO_CONNECTED
	// GPIO_STATE[]
	// GPIO_DIR[]

	// Change To Input PD for Isolator detection
	for (int n = 1 ; n <= MAX_GPIO; n++) {
		set_gpio_input(n, IN_PD);
		updateGIPO_state(n,IN);
	}
	// Update the input state and check what is Hight
	updateGlobalDir();
	for (int i = 0 ; i < MAX_GPIO; i++) {
		if ( GPIO_DIR[i] == 1) {
			GPIO_CONNECTED[i] = CONNECTED_ISOLATOR;
		}
		else {
			GPIO_CONNECTED[i] = NOT_CONNECTED_STATE;
		}
	}
	// Cahnge to In PU and validate what is down
	// If Not connected before and now Low, probably M connected.
	for (int n = 1 ; n <= MAX_GPIO; n++) {
		set_gpio_input(n, IN_PU);
		updateGIPO_state(n,IN);
	}
	updateGlobalDir();
	for (int i = 0 ; i < MAX_GPIO; i++) {
		if (( GPIO_DIR[i] == 0) & (GPIO_CONNECTED[i] == NOT_CONNECTED_STATE)) {
			GPIO_CONNECTED[i] = CONNECTED_MOSFET;
		}
	}

	// Leave connected to Input PD, just in case
	for (int n = 1 ; n <= MAX_GPIO; n++) {
		set_gpio_input(n, IN_PD);
		updateGIPO_state(n,IN);
	}
}

/**
 * @brief	Set GPIO to the received level state
 * @param	int GPIO to be changed
 * @param	int State to be updated to 0 - In, 1 - Out
 * @retval	int Function return code
 */
int setGPIO_level(int gpio_number, int state) {
	int returnCode = 0;

	// Validate GPIO range
	if (testGPIO(gpio_number) == 0) {

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		switch (state) {
		case 0:    											// Input
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			//			GPIO_InitStruct.Pull = GPIO_PULLDOWN;
			//			write("State INPUT\r\n");
			updateGIPO_state(gpio_number,0);
			break;
		case 1: 											// Output
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			updateGIPO_state(gpio_number,1);				// Update global array for GPIO state
			break;
		default:
			write("State wrong ERROR_04\r\n");
			returnCode = ERROR_04;
			return returnCode;
			break;
		}

		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;


		//		write("set GPIO input OK\r\n");
		switch (gpio_number)

		{
		case 1:
			GPIO_InitStruct.Pin = GPIO_1_Pin;
			HAL_GPIO_Init(GPIO_1_GPIO_Port, &GPIO_InitStruct);
			break;
		case 2:
			GPIO_InitStruct.Pin = GPIO_2_Pin;
			HAL_GPIO_Init(GPIO_2_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 3:
			GPIO_InitStruct.Pin = GPIO_3_Pin;
			HAL_GPIO_Init(GPIO_3_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 4:
			GPIO_InitStruct.Pin = GPIO_4_Pin;
			HAL_GPIO_Init(GPIO_4_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 5:
			GPIO_InitStruct.Pin = GPIO_5_Pin;
			HAL_GPIO_Init(GPIO_5_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 6:
			GPIO_InitStruct.Pin = GPIO_6_Pin;
			HAL_GPIO_Init(GPIO_6_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 7:
			GPIO_InitStruct.Pin = GPIO_7_Pin;
			HAL_GPIO_Init(GPIO_7_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 8:
			GPIO_InitStruct.Pin = GPIO_8_Pin;
			HAL_GPIO_Init(GPIO_8_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 9:
			GPIO_InitStruct.Pin = GPIO_9_Pin;
			HAL_GPIO_Init(GPIO_9_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 10:
			GPIO_InitStruct.Pin = GPIO_10_Pin;
			HAL_GPIO_Init(GPIO_10_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 11:
			GPIO_InitStruct.Pin = GPIO_11_Pin;
			HAL_GPIO_Init(GPIO_11_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 12:
			GPIO_InitStruct.Pin = GPIO_12_Pin;
			HAL_GPIO_Init(GPIO_12_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 13:
			GPIO_InitStruct.Pin = GPIO_13_Pin;
			HAL_GPIO_Init(GPIO_13_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 14:
			GPIO_InitStruct.Pin = GPIO_14_Pin;
			HAL_GPIO_Init(GPIO_14_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 15:
			GPIO_InitStruct.Pin = GPIO_15_Pin;
			HAL_GPIO_Init(GPIO_15_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 16:
			GPIO_InitStruct.Pin = GPIO_16_Pin;
			HAL_GPIO_Init(GPIO_16_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 17:
			GPIO_InitStruct.Pin = GPIO_17_Pin;
			HAL_GPIO_Init(GPIO_17_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 18:
			GPIO_InitStruct.Pin = GPIO_18_Pin;
			HAL_GPIO_Init(GPIO_18_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 19:
			GPIO_InitStruct.Pin = GPIO_19_Pin;
			HAL_GPIO_Init(GPIO_19_GPIO_Port,  &GPIO_InitStruct);
			break;
		case 20:
			GPIO_InitStruct.Pin = GPIO_20_Pin;
			HAL_GPIO_Init(GPIO_20_GPIO_Port,  &GPIO_InitStruct);
			break;
		}
	}
	else {
		returnCode = ERROR_03;
		write("Set GPIO Input Number wrong\r\n ");
	}
	return returnCode;
}

/**
 * @brief  Print the global State of Each GPIO pin
 *
 *
 *        @note
 *        Print from the global array GPIO_STATE
 *        The printout is in the following format:
 *        	1 - GPIO is in Output state
 *        	2 - GPIO is in Input state
 *        	x - Not Connected
 */
void printConnected() {

	write(" ======= Isolation connected to TEL-GPIO ======= \r\n");
	write("I - Connected Isolation Chip ADM3260ARSZ \r\n");
	write("M - Connected Isolation that can drive current without Input capability\r\n");
	write("X - Not connected Isolation\r\n");
	char GPIO_CONNECTED_PRINT[70] = "GPIO Connected (I,M):\t";



	for (int n = 0 ; n < MAX_GPIO ; n++) {

		switch (GPIO_CONNECTED[n])
		{

		case NOT_CONNECTED_STATE: // Not Connected
			strcat(GPIO_CONNECTED_PRINT,"X");
			break;
		case CONNECTED_ISOLATOR:	// Connected ISOLATOR
			strcat(GPIO_CONNECTED_PRINT,"I");
			break;
		case CONNECTED_MOSFET:	// Connected MOSFET
			strcat(GPIO_CONNECTED_PRINT,"M");
			break;
		default:	// Error pin case
			strcat(GPIO_CONNECTED_PRINT,"E");

		}
		if (n < MAX_GPIO-1)
			strcat(GPIO_CONNECTED_PRINT,",");

	}

	strcat(GPIO_CONNECTED_PRINT, "\r\n");
	write(GPIO_CONNECTED_PRINT);

}

/**
 * @brief  Configures EXTI line 0 (connected to PA.00 pin) in interrupt mode
 * @param  None
 * @retval None
 */
static void EXTI0_1_IRQHandler_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOA clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* Configure PA.00 pin as input floating */
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	//  GPIO_InitStructure GPIO_MODE_IT_RISING
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable and set EXTI line 0 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI0_1_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
}

/**
 * @brief EXTI line detection callbacks
 * @param GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0)
	{
		/* Toggle LED3 */
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

		//	  write("+1\r\n");

	}
	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
}

///**
// * @brief	Parse recive pointer to a GPIO command
// * @param	command -
// * @param	state - State to be updated to 0 - In, 1 - Out
// * @retval	None
// */
//int parseCommand(){
//
//}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
