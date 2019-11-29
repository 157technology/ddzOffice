#ifndef __LED_CONFIG_H__
#define __LED_CONFIG_H__ 

#include "main.h"


#define LED1	LED2_GPIO_Port, LED2_Pin
#define LED2	LED1_GPIO_Port, LED1_Pin
#define LED3	LED3_GPIO_Port, LED3_Pin
#define LED4	LED4_GPIO_Port, LED4_Pin


#define LED_Set(port, pin, state)		HAL_GPIO_WritePin((port), (pin), (GPIO_PinState)(state))
#define LED_Toggle(port, pin)			HAL_GPIO_TogglePin((port), (pin))


#ifdef USING_OS
	#define LED_Delay 	osDelay
#else
	#define LED_Delay	HAL_Delay
#endif



#endif