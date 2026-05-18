/*
 * msp.c
 *
 *  Created on: 18 May 2026
 *      Author: MONSTER
 */

#include "main.h"


void HAL_MspInit(void)
{
	// Here will do low level processor specific inits
	// 1. Set up the priority grouping of the ARM Cortex MX processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the ARM Cortex MX processor
	SCB->SHCSR |= 0x7 << 16;		// Usage fault, memory fault and bus fault system exceptions

	// 3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_Uart;
	// Here we are going to do the low level inits. of the USART2 peripheral
	// 1. Enable the clock for the USART2 and GPIOA peripheral
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 2. Do the pin muxing configuration
	gpio_Uart.Pin = GPIO_PIN_2;
	gpio_Uart.Mode = GPIO_MODE_AF_PP;
	gpio_Uart.Pull = GPIO_PULLUP;
	gpio_Uart.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_Uart.Alternate = GPIO_AF7_USART2; 		// USART_TX
	HAL_GPIO_Init(GPIOA, &gpio_Uart);

	gpio_Uart.Pin = GPIO_PIN_3;					// USART_RX
	HAL_GPIO_Init(GPIOA, &gpio_Uart);

	// 3. Enable the IRQ and set up the priority (NVIC settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
	// 1. Enable the clock for the TIM6 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();

	// 2. Enable the IRQ of TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	// 3. Configure the priority for the TIM6_DAC_IRQ
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);
}


void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef gpio_Can;

	// 1. Enable the clock for the CAN1 and GPIOD peripherals
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	// CAN1 GPIO Configuration
	// PD0 ---> CAN1_RX
	// PD1 ---> CAN1_TX
	// 2. Do the pin muxing configuration
	gpio_Can.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	gpio_Can.Mode = GPIO_MODE_AF_PP;
	gpio_Can.Pull = GPIO_PULLUP;
	gpio_Can.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_Can.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOD, &gpio_Can);

	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15, 0);

	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

}


