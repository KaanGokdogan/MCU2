/*
 * it.c
 *
 *  Created on: 27 Nis 2026
 *      Author: MONSTER
 */

#include "it.h"

extern UART_HandleTypeDef huart2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}
