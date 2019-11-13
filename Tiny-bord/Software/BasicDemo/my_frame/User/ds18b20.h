#ifndef __DS18B20_H__
#define __DS18B20_H__ 

#include "main.h"

typedef enum
{
  Pin_OFF = 0,
  Pin_ON
}Pin_State;

typedef struct pin_s
{
	GPIO_TypeDef * port;
	uint16_t		pin;
}pin_t;

typedef struct temper_s
{
	float (*temper)	(void);
}temper_t;


extern temper_t Temper18b20;

#endif