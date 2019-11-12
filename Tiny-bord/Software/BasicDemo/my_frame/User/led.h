#ifndef __LED_H__
#define __LED_H__ 


#include "main.h"

typedef enum
{
  LED_OFF = 0,
  LED_ON
}LED_State;

typedef struct led_s
{
	GPIO_TypeDef * port;
	uint16_t		pin;
}led_t;

typedef struct led_drv_s
{
	uint16_t num;	// 个数
	led_t * led;
	void (* set)			(struct led_drv_s * this, uint16_t lednum, LED_State s);
	void (* toggle)			(struct led_drv_s * this, uint16_t lednum);
	void (* leftstream)		(struct led_drv_s * this, uint16_t v);
	void (* rightstream)	(struct led_drv_s * this, uint16_t v);
}led_drv_t;



extern led_drv_t ledDrv;

#endif