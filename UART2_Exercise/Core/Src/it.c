/*
 * it.c
 *
 *  Created on: 8 Nis 2026
 *      Author: MONSTER
 */

#include "stm32f4xx_hal.h"
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
