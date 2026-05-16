/*
 * main.h
 *
 *  Created on: 14 May 2026
 *      Author: MONSTER
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#define SYS_CLOCK_FREQ_50_MHZ		50
#define SYS_CLOCK_FREQ_84_MHZ		84
#define SYS_CLOCK_FREQ_120_MHZ		120
#define SYS_CLOCK_FREQ_168_MHZ		168



void SystemClock_Config_HSE(uint8_t clock_Freq);
void GPIO_Init();
void UART2_Init(void);
void CAN1_Init(void);
void CAN1_Tx(void);
void CAN_Filter_Config(void);


void ErrorHandler(void);


#endif /* INC_MAIN_H_ */
