/*
 * main.c
 *
 *  Created on: 4 May 2026
 *      Author: MONSTER
 */

#include "main.h"

UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;
TIM_HandleTypeDef htimer6;

uint32_t input_Capture[2] = {0};
uint8_t count = 1;
uint8_t is_Capture_Done = FALSE;


int main(void)
{
	uint32_t capture_Difference = 0;
	double timer2_Count_Freq = 0;
	double timer2_Count_Reset = 0;
	double user_Signal_Time_Period = 0;
	double user_Signal_Freq = 0;
	char user_Msg[100];


	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);
	UART2_Init();
	GPIO_Init();
	Timer2_Init();
	Timer6_Init();
	LSE_Config();


	HAL_TIM_Base_Start_IT(&htimer6);

	HAL_TIM_IC_Start_IT(&htimer2, TIM_CHANNEL_1);

	while(1)
	{
		if(is_Capture_Done)
		{
			if(input_Capture[1] > input_Capture[0])
				capture_Difference = input_Capture[1] - input_Capture[0];
			else
				capture_Difference = (0xFFFFFFFF - input_Capture[0]) + input_Capture[1];

		timer2_Count_Freq =  (HAL_RCC_GetPCLK1Freq() * 2) / (htimer2.Init.Prescaler + 1);
		timer2_Count_Reset = 1.0 / timer2_Count_Freq;
		user_Signal_Time_Period = capture_Difference * timer2_Count_Reset;
		user_Signal_Freq = 1.0 / user_Signal_Time_Period;
		sprintf(user_Msg,"Frequency of the signal applied = %f\r\n", user_Signal_Freq);
		HAL_UART_Transmit(&huart2, (uint8_t*) user_Msg, strlen(user_Msg), HAL_MAX_DELAY);

		is_Capture_Done = FALSE;
		}
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
	htimer2.Init.CounterMode =TIM_COUNTERMODE_UP;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1;

	if( HAL_TIM_IC_Init(&htimer2) != HAL_OK )
	{
		ErrorHandler();
	}

	TIM_IC_InitTypeDef timer2IC_Config;
	timer2IC_Config.ICFilter = 0;
	timer2IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	timer2IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	timer2IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;

	if( HAL_TIM_IC_ConfigChannel(&htimer2, &timer2IC_Config, TIM_CHANNEL_1) != HAL_OK)
	{
		ErrorHandler();
	}
}

void Timer6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 99;
	htimer6.Init.Period = 31250 - 1;

	if(HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		ErrorHandler();
	}
}


void LSE_Config (void)
{
	// Board does not include an LSE crystal. Replaced with HSI
	// And we put jumper cable to PA0 and PD13 (Led)
#if 0
	RCC_OscInitTypeDef osc_Init;
	osc_Init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	osc_Init.LSEState = RCC_LSE_ON;

	if(HAL_RCC_OscConfig(&osc_Init) != HAL_OK)
	{
		ErrorHandler();
	}

#endif
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_4);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(! is_Capture_Done)
	{
		if(count == 1)
		{
		  input_Capture[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
		  count++;
		}
		else if(count == 2)
		{
		  input_Capture[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
		  count = 1;
		  is_Capture_Done = TRUE;
		}
	}

}

void ErrorHandler(void)
{
	while(1);
}

