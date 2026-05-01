/*
 * main.c
 *
 *  Created on: 29 Nis 2026
 *      Author: MONSTER
 */

#include "main.h"


TIM_HandleTypeDef htimer6;


int main(void)
{
	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	Timer6_Init();

	// Start the timer
	HAL_TIM_Base_Start_IT(&htimer6);




	return 0;
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


void Timer6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 24;
	htimer6.Init.Period = 64000 - 1;

	if(HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		ErrorHandler();
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
}


void SystemClockConfig(void)
{

}

void ErrorHandler(void)
{
	while(1);
}


