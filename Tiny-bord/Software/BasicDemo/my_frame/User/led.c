#include "led.h"

#include "led_config.h"

led_t m_leds[] = 
{
	{LED1},
	{LED2},
	{LED3},
	{LED4}
};



static void led_set(led_drv_t * this, uint16_t lednum, LED_State s)
{
	LED_Set(m_leds[lednum-1].port, m_leds[lednum-1].pin, (GPIO_PinState)s);
}
static void led_toggle(led_drv_t * this, uint16_t lednum)
{
	LED_Toggle(m_leds[lednum-1].port, m_leds[lednum-1].pin);
}
static void led_leftstream(led_drv_t * this, uint16_t v)
{
	static uint16_t pos = 0;
	LED_Toggle(m_leds[pos].port, m_leds[pos].pin);
	if ( pos ++  == this->num )	pos = 0;
	
	LED_Delay(v);
}
static void led_rightstream(led_drv_t * this, uint16_t v)
{
	static int f = 0;
	static uint16_t pos = 0;
	
	if ( f ==  0 )
	{
		pos = this->num - 1;
		f = 1;
	}
	
	LED_Toggle(m_leds[pos].port, m_leds[pos].pin);
	if ( pos == 0 )	pos = this->num - 1;
	else			pos --;
	
	LED_Delay(v);
}

led_drv_t ledDrv = 
{
	sizeof(m_leds)/sizeof(led_t),
	m_leds,
	led_set,
	led_toggle,
	led_leftstream,
	led_rightstream
};