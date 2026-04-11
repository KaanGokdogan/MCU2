/*
 * main.c
 *
 *  Created on: 8 Nis 2026
 *      Author: MONSTER
 */

#include "main.h"
#include "stdint.h"
#include "string.h"



char *userData = "The application is running./r/n";


int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	uint16_t strLen = strlen(userData);
	HAL_UART_Transmit(&huart2, (uint8_t*) userData, strLen, HAL_MAX_DELAY);

	while(1);

	return 0;
}



void SystemClockConfig(void)
{

}


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		// There is a problem
		ErrorHandler();
	}
}


void ErrorHandler(void)
{
	while(1);
}
