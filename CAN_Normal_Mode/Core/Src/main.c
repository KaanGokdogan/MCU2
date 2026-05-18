/*
 * main.c
 *
 *  Created on: 18 May 2026
 *      Author: MONSTER
 */

#include "main.h"

TIM_HandleTypeDef htimer6;
UART_HandleTypeDef huart2;
CAN_HandleTypeDef hcan1;

uint8_t led_Number = 0;
uint8_t req_Counter = 0;
CAN_RxHeaderTypeDef RxHeader;


int main(void)
{
	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();
	Timer6_Init();
	UART2_Init();
	CAN1_Init();
	CAN_Filter_Config();


	if( HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF) != HAL_OK)
	{
		// There is a problem
		ErrorHandler();
	}

	if( HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		// There is a problem
		ErrorHandler();
	}

	//CAN1_Tx(); // 1 time testing

	while(1);
	return 0;
}


void SystemClock_Config_HSE(uint8_t clock_Freq)
{
	RCC_OscInitTypeDef osc_Init;
	uint32_t flash_Latency = 0;

	osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_Init.HSEState = RCC_HSE_BYPASS;
	osc_Init.PLL.PLLState = RCC_PLL_ON;
	osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	RCC_ClkInitTypeDef clock_Config;
	clock_Config.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
									RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clock_Config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

	switch(clock_Freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_Init.PLL.PLLM = 8;
			osc_Init.PLL.PLLN = 100;
			osc_Init.PLL.PLLP = 2;
			osc_Init.PLL.PLLQ = 2;

			clock_Config.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clock_Config.APB1CLKDivider = RCC_HCLK_DIV2;
			clock_Config.APB2CLKDivider = RCC_HCLK_DIV2;

			flash_Latency = FLASH_ACR_LATENCY_1WS;
			break;
		}
		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_Init.PLL.PLLM = 8;
			osc_Init.PLL.PLLN = 168;
			osc_Init.PLL.PLLP = 2;
			osc_Init.PLL.PLLQ = 2;

			clock_Config.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clock_Config.APB1CLKDivider = RCC_HCLK_DIV2;
			clock_Config.APB2CLKDivider = RCC_HCLK_DIV2;

			flash_Latency = FLASH_ACR_LATENCY_2WS;
			break;
		}
		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_Init.PLL.PLLM = 8;
			osc_Init.PLL.PLLN = 240;
			osc_Init.PLL.PLLP = 2;
			osc_Init.PLL.PLLQ = 2;

			clock_Config.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clock_Config.APB1CLKDivider = RCC_HCLK_DIV4;
			clock_Config.APB2CLKDivider = RCC_HCLK_DIV2;

			flash_Latency = FLASH_ACR_LATENCY_3WS;
			break;
		}
		case SYS_CLOCK_FREQ_168_MHZ:
		{
			// Enable the clock for the power controller
			__HAL_RCC_PWR_CLK_ENABLE();

			// Set regulator voltage scale as 1
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

			osc_Init.PLL.PLLM = 8;
			osc_Init.PLL.PLLN = 336;
			osc_Init.PLL.PLLP = 2;
			osc_Init.PLL.PLLQ = 2;

			clock_Config.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clock_Config.APB1CLKDivider = RCC_HCLK_DIV4;
			clock_Config.APB2CLKDivider = RCC_HCLK_DIV2;

			flash_Latency = FLASH_ACR_LATENCY_5WS;
			break;
		}
		default:
			return;
	}

	if(HAL_RCC_OscConfig(&osc_Init) != HAL_OK)
	{
		// There is a problem
		ErrorHandler();
	}

	if(HAL_RCC_ClockConfig(&clock_Config, flash_Latency) != HAL_OK)
	{
		// There is a problem
		ErrorHandler();
	}

	// Configure the SysTick timer interrupt frequency (for every 1 ms)
	uint32_t hclk_Freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_Freq / 1000);

	// SysTick configuration
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	// SysTick_IRQn interrupt configuration
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void GPIO_Init()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef gpioLed;
	gpioLed.Pin = GPIO_PIN_13;
	gpioLed.Mode = GPIO_MODE_OUTPUT_PP;
	gpioLed.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &gpioLed);

	gpioLed.Pin = GPIO_PIN_0;
	gpioLed.Mode = GPIO_MODE_IT_FALLING;
	gpioLed.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpioLed);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);


}


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		// There is a problem
		ErrorHandler();
	}
}


void Timer6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 4999;
	htimer6.Init.Period = 10000 - 1;

	if(HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		ErrorHandler();
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailBox;
	uint8_t message;	// No meaning for data frame

	if(req_Counter == 4)
	{
		// N1 sending remote frame to n2
		TxHeader.DLC = 2;		// N1 demanding 2 bytes of reply
		TxHeader.StdId = 0x651;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.RTR = CAN_RTR_REMOTE;

		if( HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message, &TxMailBox) != HAL_OK)
		{
			ErrorHandler();
		}

		req_Counter = 0;
	}
	else
	{
		CAN1_Tx();
		req_Counter++;
	}

}


void CAN1_Init(void)
{
	hcan1.Instance = CAN1;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.AutoBusOff = ENABLE;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;

	// Settings related to CAN bit timings
	hcan1.Init.Prescaler = 5;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan1.Init.TimeSeg2 = CAN_BS1_1TQ;

	if(HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		// There is a problem
		ErrorHandler();
	}
}


void CAN1_Tx(void)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailBox;
	uint8_t message;


	TxHeader.DLC = 5;
	TxHeader.StdId = 0x65D;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;

	message = ++led_Number;

	if( led_Number == 4)
	{
		led_Number = 0;
	}

	if( HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message, &TxMailBox) != HAL_OK)
	{
		ErrorHandler();
	}
}
void CAN1_Rx(void)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailBox;
	uint8_t message;


	TxHeader.DLC = 5;
	TxHeader.StdId = 0x65D;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;

	message = ++led_Number;

	if( led_Number == 4)
	{
		led_Number = 0;
	}

	if( HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message, &TxMailBox) != HAL_OK)
	{
		ErrorHandler();
	}
}


void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef can1_Filter_Init;

	can1_Filter_Init.FilterActivation = ENABLE;
	can1_Filter_Init.FilterBank = 0;
	can1_Filter_Init.FilterFIFOAssignment = CAN_RX_FIFO0;
	can1_Filter_Init.FilterIdHigh = 0x0000;
	can1_Filter_Init.FilterIdLow = 0x0000;
	can1_Filter_Init.FilterMaskIdHigh = 0x0000;
	can1_Filter_Init.FilterMaskIdLow = 0x0000;
	can1_Filter_Init.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_Filter_Init.FilterScale = CAN_FILTERSCALE_32BIT;

	if( HAL_CAN_ConfigFilter(&hcan1, &can1_Filter_Init) != HAL_OK)
	{
		// There is a problem
		ErrorHandler();
	}
}


void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	char buffer[50];

	sprintf(buffer,"Message transmitted: M0\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
}


void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	char buffer[50];

	sprintf(buffer,"Message transmitted: M1\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
}


void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	char buffer[50];

	sprintf(buffer,"Message transmitted: M2\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t message[9];
	char buffer[50];

	if( HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, message) != HAL_OK)
	{
		// There is a problem
		ErrorHandler();
	}

	if( RxHeader.StdId == 0x65D && RxHeader.RTR == 0)
	{
		// This is data frame sent by n1 to n2
		LED_Manage_Output(message[0]);
		sprintf(buffer,"Message received: #%x\r\n",message[0]);
	}
	else if( RxHeader.StdId == 0x651 && RxHeader.RTR == 1)
	{
		// This is remote frame sent by n1 to n2
		Send_Response(RxHeader.StdId);
		return;
	}
	else if( RxHeader.StdId == 0x651 && RxHeader.RTR == 0)
	{
		// its a reply (data frame) by n2 to n1
		sprintf(buffer,"Reply received: #%X\r\n",message[0] << 8 | message[1]);
	}

	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
}


void LED_Manage_Output(uint8_t Led_Number)
{
	switch(led_Number)
	{
	case 1:
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_SET);
		break;
	}
}


void Send_Response(uint32_t StdId)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailBox;
	uint8_t response[2] = { 0xAB, 0xCD};

	TxHeader.DLC = 2;
	TxHeader.StdId = StdId;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;

	if( HAL_CAN_AddTxMessage(&hcan1, &TxHeader, response, &TxMailBox) != HAL_OK)
	{
		ErrorHandler();
	}
}


void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	char buffer[50];

	sprintf(buffer,"CAN Error Detected!\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
}



void ErrorHandler(void)
{
	while(1);
}


