/*
 * it.c
 *
 *  Created on: 29 Nis 2026
 *      Author: MONSTER
 */

#include "it.h"


void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
