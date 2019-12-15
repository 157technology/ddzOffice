#ifndef __KEY_CONFIG_H__
#define __KEY_CONFIG_H__ 

#include "main.h"
#include "gpio.h"



#define GET_KEY_LEFT()		HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin)
#define GET_KEY_RIGHT()		HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin)

#endif