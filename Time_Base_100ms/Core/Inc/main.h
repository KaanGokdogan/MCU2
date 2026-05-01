/*
 * main.h
 *
 *  Created on: 29 Nis 2026
 *      Author: MONSTER
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "string.h"

void SystemClockConfig(void);
void GPIO_Init();
void Timer6_Init(void);
void ErrorHandler(void);


#endif /* INC_MAIN_H_ */
