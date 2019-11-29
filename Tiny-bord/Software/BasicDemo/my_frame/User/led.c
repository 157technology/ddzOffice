#include "led.h"

#include "led_config.h"

led_t m_leds[] =
{
	{LED1},
	{LED2},
	{LED3},
	{LED4}
};


/**
 * [led_set description]
 * @Author   EMei_Li
 * @DateTime 2019-11-19T11:00:16+0800
 * @param    lednum                   [description]
 * @param    s                        [description]
 */
static void led_set(uint16_t lednum, LED_State s)
{
	LED_Set(m_leds[lednum - 1].port, m_leds[lednum - 1].pin, (GPIO_PinState)s);
}

static void led_toggle(uint16_t lednum)
{
	LED_Toggle(m_leds[lednum - 1].port, m_leds[lednum - 1].pin);
}

static void led_leftstream(uint16_t v)
{
	for ( int i = 0; i < ledDrv.num; i ++ )
	{
		LED_Set(m_leds[i].port, m_leds[i].pin, LED_OFF);
	}

	for ( int i = 0; i < ledDrv.num; i ++ )
	{
		LED_Toggle(m_leds[i].port, m_leds[i].pin);
		LED_Delay(v);
	}

	for ( int i = 0; i < ledDrv.num; i ++ )
	{
		LED_Toggle(m_leds[i].port, m_leds[i].pin);
		LED_Delay(v);
	}
}

static void led_rightstream(uint16_t v)
{
	for ( int i = ledDrv.num - 1; i >= 0; i -- )
	{
		LED_Set(m_leds[i].port, m_leds[i].pin, LED_OFF);
	}

	for ( int i = ledDrv.num - 1; i >= 0; i -- )
	{
		LED_Toggle(m_leds[i].port, m_leds[i].pin);
		LED_Delay(v);
	}

	for ( int i = ledDrv.num - 1; i >= 0; i -- )
	{
		LED_Toggle(m_leds[i].port, m_leds[i].pin);
		LED_Delay(v);
	}
}

led_drv_t ledDrv =
{
	sizeof(m_leds) / sizeof(led_t),
	m_leds,
	led_set,
	led_toggle,
	led_leftstream,
	led_rightstream
};