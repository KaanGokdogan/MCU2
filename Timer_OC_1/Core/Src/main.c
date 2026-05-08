/*
 * main.c
 *
 *  Created on: 4 May 2026
 *      Author: MONSTER
 */

#include "main.h"

UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;

uint32_t pulse1_Value = 25000;  // to produce 500Hz
uint32_t pulse2_Value = 12500;	// to produce 1000Hz
uint32_t pulse3_Value = 6250;	// to produce 2000Hz
uint32_t pulse4_Value = 3125;	// to produce 4000Hz

uint32_t ccr_Content;



int main(void)
{

	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);
	UART2_Init();
	GPIO_Init();
	Timer2_Init();

	if( HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK)
	{
		ErrorHandler();
	}

	if( HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK)
	{
		ErrorHandler();
	}

	if( HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK)
	{
		ErrorHandler();
	}

	if( HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK)
	{
		ErrorHandler();
	}

	while(1)
	{

	}

	return 0;
}


void SystemClockConfig(uint8_t clock_Freq)
{
	RCC_OscInitTypeDef osc_Init;
	RCC_ClkInitTypeDef clock_Config;

	uint32_t flash_Latency = 0;

	osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_Init.HSIState = RCC_HSI_ON;		// default HSI is on
	osc_Init.HSICalibrationValue = 16;
	osc_Init.PLL.PLLState = RCC_PLL_ON;
	osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	clock_Config.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
									RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clock_Config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

	switch(clock_Freq)
	{
	case SYS_CLOCK_FREQ_50_MHZ:
	{
		osc_Init.PLL.PLLM = 16;
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
		osc_Init.PLL.PLLM = 16;
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
		osc_Init.PLL.PLLM = 16;
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

			osc_Init.PLL.PLLM = 16;
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

	// Configure the SysTick timer interrupt frequency (for every 1 ms)
	uint32_t hclk_Freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_Freq / 1000);

	// SysTick configuration
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	// SysTick_IRQn interrupt configuration
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
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


void GPIO_Init()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpioLed;
	gpioLed.Mode = GPIO_MODE_OUTPUT_PP;
	gpioLed.Pin = GPIO_PIN_13;
	gpioLed.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &gpioLed);
}


void Timer2_Init(void)
{
	htimer2.Instance = TIM2;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1;

	if( HAL_TIM_OC_Init(&htimer2) != HAL_OK )
	{
		ErrorHandler();
	}

	TIM_OC_InitTypeDef timer2OC_Config;
	timer2OC_Config.OCMode = TIM_OCMODE_TOGGLE;
	timer2OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;

	timer2OC_Config.Pulse = pulse1_Value;
	if( HAL_TIM_OC_ConfigChannel(&htimer2, &timer2OC_Config, TIM_CHANNEL_1) != HAL_OK)
	{
		ErrorHandler();
	}

	timer2OC_Config.Pulse = pulse2_Value;
	if( HAL_TIM_OC_ConfigChannel(&htimer2, &timer2OC_Config, TIM_CHANNEL_2) != HAL_OK)
	{
		ErrorHandler();
	}

	timer2OC_Config.Pulse = pulse3_Value;
	if( HAL_TIM_OC_ConfigChannel(&htimer2, &timer2OC_Config, TIM_CHANNEL_3) != HAL_OK)
	{
		ErrorHandler();
	}

	timer2OC_Config.Pulse = pulse4_Value;
	if( HAL_TIM_OC_ConfigChannel(&htimer2, &timer2OC_Config, TIM_CHANNEL_4) != HAL_OK)
	{
		ErrorHandler();
	}

}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	// TIM2_CH1 toggling with frequency = 500 Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		ccr_Content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_Content + pulse1_Value);
	}

	// TIM2_CH2 toggling with frequency = 1000 Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		ccr_Content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, ccr_Content + pulse2_Value);
	}

	// TIM2_CH3 toggling with frequency = 2000 Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		ccr_Content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, ccr_Content + pulse3_Value);
	}

	// TIM2_CH4 toggling with frequency = 4000 Hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		ccr_Content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, ccr_Content + pulse4_Value);
	}

}



void ErrorHandler(void)
{
	while(1);
}

