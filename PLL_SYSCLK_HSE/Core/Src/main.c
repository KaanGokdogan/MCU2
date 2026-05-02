/*
 * main.c
 *
 *  Created on: 27 Nis 2026
 *      Author: MONSTER
 */

#include "main.h"


UART_HandleTypeDef huart2;


int main(void)
{
	char data[100];
	HAL_Init();

	SystemClock_Config_HSI(SYS_CLOCK_FREQ_50_MHZ);

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


void SystemClock_Config_HSI(uint8_t clock_Freq)
{
	RCC_OscInitTypeDef osc_Init;
	RCC_ClkInitTypeDef clock_Config;

	uint32_t flash_Latency = 0;

	osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_Init.HSEState = RCC_HSE_BYPASS;
	osc_Init.PLL.PLLState = RCC_PLL_ON;
	osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	clock_Config.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
									RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clock_Config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

	switch(clock_Freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_Init.PLL.PLLM = 8;
			osc_Init.PLL.PLLN = 100;
			osc_Init.PLL.PLLP = 2;
			osc_Init.PLL.PLLQ = 2;

			clock_Config.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clock_Config.APB1CLKDivider = RCC_HCLK_DIV2;
			clock_Config.APB2CLKDivider = RCC_HCLK_DIV2;

			flash_Latency = FLASH_ACR_LATENCY_1WS;
			break;
		}
		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_Init.PLL.PLLM = 8;
			osc_Init.PLL.PLLN = 168;
			osc_Init.PLL.PLLP = 2;
			osc_Init.PLL.PLLQ = 2;

			clock_Config.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clock_Config.APB1CLKDivider = RCC_HCLK_DIV2;
			clock_Config.APB2CLKDivider = RCC_HCLK_DIV2;

			flash_Latency = FLASH_ACR_LATENCY_2WS;
			break;
		}
		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_Init.PLL.PLLM = 8;
			osc_Init.PLL.PLLN = 240;
			osc_Init.PLL.PLLP = 2;
			osc_Init.PLL.PLLQ = 2;

			clock_Config.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clock_Config.APB1CLKDivider = RCC_HCLK_DIV4;
			clock_Config.APB2CLKDivider = RCC_HCLK_DIV2;

			flash_Latency = FLASH_ACR_LATENCY_3WS;
			break;
		}
		case SYS_CLOCK_FREQ_168_MHZ:
		{
			// Enable the clock for the power controller
			__HAL_RCC_PWR_CLK_ENABLE();

			// Set regulator voltage scale as 1
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

			osc_Init.PLL.PLLM = 8;
			osc_Init.PLL.PLLN = 336;
			osc_Init.PLL.PLLP = 2;
			osc_Init.PLL.PLLQ = 2;

			clock_Config.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clock_Config.APB1CLKDivider = RCC_HCLK_DIV4;
			clock_Config.APB2CLKDivider = RCC_HCLK_DIV2;

			flash_Latency = FLASH_ACR_LATENCY_5WS;
			break;
		}
		default:
			return;
	}


	if(HAL_RCC_OscConfig(&osc_Init) != HAL_OK)
	{
		ErrorHandler();
	}

	if(HAL_RCC_ClockConfig(&clock_Config, flash_Latency) != HAL_OK)
	{
		ErrorHandler();
	}

	// Systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
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


