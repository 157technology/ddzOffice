#include "led.h"




GPIO_TypeDef * ledPortList[4] = {LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port};
uint16_t	 ledPinList[4]  = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin};

void ledLoop(uint16_t time)
{
//	for (int i = 0; i < 4; ++i)
//	{
//		for (int j = 0; j < 4; ++j)
//		{
//			if ( i == j )
//				HAL_GPIO_WritePin(ledPortList[j], ledPinList[j], GPIO_PIN_SET);
//			else
//				HAL_GPIO_WritePin(ledPortList[j], ledPinList[j], GPIO_PIN_RESET);
//		}
//		HAL_Delay(time);
//	}
	
	
}

void ledBlingBling(uint16_t time)
{
	static int i = 1;

	if ( i == 1 )
	{
		i = 0;
		HAL_GPIO_WritePin(ledPortList[0], ledPinList[0], GPIO_PIN_SET);
		HAL_GPIO_WritePin(ledPortList[1], ledPinList[1], GPIO_PIN_SET);
		HAL_GPIO_WritePin(ledPortList[2], ledPinList[2], GPIO_PIN_SET);
		HAL_GPIO_WritePin(ledPortList[3], ledPinList[3], GPIO_PIN_SET);
	}

	for (int i = 0; i < 4; ++i)
	{
		HAL_GPIO_TogglePin(ledPortList[i], ledPinList[i]);
	}
	
	HAL_Delay(time);
}

