/*
 * main.h
 *
 *  Created on: 4 May 2026
 *      Author: MONSTER
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>


#define SYS_CLOCK_FREQ_50_MHZ			50
#define SYS_CLOCK_FREQ_84_MHZ			84
#define SYS_CLOCK_FREQ_120_MHZ			120
#define SYS_CLOCK_FREQ_168_MHZ			168

#define TRUE	1
#define FALSE	0


void SystemClock_Config_HSE(uint8_t clock_Freq);
void UART2_Init(void);
void GPIO_Init();
void Timer2_Init(void);
void ErrorHandler(void);



#endif /* INC_MAIN_H_ */
