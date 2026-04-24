/*
 * main.c
 *
 *  Created on: 8 Nis 2026
 *      Author: MONSTER
 */

#include "main.h"
#include "stdint.h"
#include "string.h"

#define TRUE 	1
#define FALSE 	0

UART_HandleTypeDef huart2;
char *userData = "The application is running./r/n";

uint8_t received_Data;
uint8_t reception_Complete;
uint8_t data_Buffer[100];
uint32_t count = 0;


int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	uint16_t strLen = strlen(userData);
	HAL_UART_Transmit(&huart2, (uint8_t*) userData, strLen, HAL_MAX_DELAY);

	HAL_UART_Receive_IT(&huart2, &received_Data, 1);

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


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(received_Data == '/r')
	{
		reception_Complete = TRUE;
		data_Buffer[count++] = 'r';
		HAL_UART_Transmit(&huart2, data_Buffer, 1, HAL_MAX_DELAY);
	}
	else
	{
		data_Buffer[count++] = received_Data;
	}
}


uint8_t Convert_To_Capital(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		data = data - ( 'a' - 'A');
	}

	return data;
}


void ErrorHandler(void)
{
	while(1);
}


