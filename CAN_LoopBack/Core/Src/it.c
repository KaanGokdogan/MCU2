/*
 * it.c
 *
 *  Created on: 14 May 2026
 *      Author: MONSTER
 */

#include "it.h"

extern UART_HandleTypeDef huart2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
