/*
 * main.c
 *
 *  Created on: 24 Nis 2026
 *      Author: MONSTER
 */

#include "main.h"


UART_HandleTypeDef huart2;



int main(void)
{
	RCC_OscInitTypeDef osc_Init;
	RCC_ClkInitTypeDef clock_Config;
	char data[100];

	HAL_Init();
	UART2_Init();

	memset(&osc_Init,0,sizeof(osc_Init));
	osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_Init.HSEState = RCC_HSE_BYPASS;

	if(HAL_RCC_OscConfig(&osc_Init) != HAL_OK)
	{
		ErrorHandler();
	}

	clock_Config.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
							RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clock_Config.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clock_Config.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clock_Config.APB1CLKDivider = RCC_HCLK_DIV2;
	clock_Config.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&clock_Config, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		ErrorHandler();
	}

	/*------------------- AFTER THIS LINE SYSCLK is SOURCED BY HSE -------------------*/

	__HAL_RCC_HSI_DISABLE();

	/* LETS REDO THE SYSTICK CONFIGURATION*/

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	UART2_Init();

	memset(&data,0,sizeof(data));
	sprintf(data,"SYSCLK: %ld\r\n",HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*) data, strlen(data), HAL_MAX_DELAY);

	memset(&data,0,sizeof(data));
	sprintf(data,"HCLK: %ld\r\n",HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*) data, strlen(data), HAL_MAX_DELAY);

	memset(&data,0,sizeof(data));
	sprintf(data,"PCLK1: %ld\r\n",HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*) data, strlen(data), HAL_MAX_DELAY);

	memset(&data,0,sizeof(data));
	sprintf(data,"PCLK2: %ld\r\n",HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*) data, strlen(data), HAL_MAX_DELAY);

	while(1);
	return 0;
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


