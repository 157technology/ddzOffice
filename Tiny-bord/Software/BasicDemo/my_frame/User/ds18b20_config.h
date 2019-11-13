#ifndef __DS18B20_CONFIG_H__
#define __DS18B20_CONFIG_H__ 

#include "main.h"

#define Pin_DS18B20	 DS18B20_GPIO_Port, DS18B20_Pin

#define Pin_Set(io, state)		HAL_GPIO_WritePin(((io).port), ((io).pin), (GPIO_PinState)(state))
#define Pin_Get(io)				HAL_GPIO_ReadPin(((io).port), ((io).pin))

#endif