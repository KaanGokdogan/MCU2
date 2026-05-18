/*
 * main.h
 *
 *  Created on: 18 May 2026
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

// define again in my board
#define LED1_PORT GPIOC
#define LED2_PORT GPIOC
#define LED3_PORT GPIOC
#define LED4_PORT GPIOC

#define LED1_PIN_NO GPIO_PIN_8
#define LED2_PIN_NO GPIO_PIN_8
#define LED3_PIN_NO GPIO_PIN_8
#define LED4_PIN_NO GPIO_PIN_8


void SystemClock_Config_HSE(uint8_t clock_Freq);
void GPIO_Init();
void Timer6_Init(void);
void UART2_Init(void);
void CAN1_Init(void);
void CAN1_Tx(void);
void CAN1_Rx(void);
void CAN_Filter_Config(void);
void LED_Manage_Output(uint8_t Led_Number);
void Send_Response(uint32_t StdId);
void ErrorHandler(void);


#endif /* INC_MAIN_H_ */
