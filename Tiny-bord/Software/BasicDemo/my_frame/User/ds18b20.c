#include "ds18b20.h"
#include "ds18b20_config.h"

#include "delay.h"

pin_t ds18b20 = { Pin_DS18B20 };

static void setInput(pin_t pin);
static void setOutput(pin_t pin);


static void setInput(pin_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = pin.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(pin.port, &GPIO_InitStruct);
}
static void setOutput(pin_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = pin.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(pin.port, &GPIO_InitStruct);
}


static uint8_t ds18b20Reset(void)
{
	setOutput(ds18b20);
	Pin_Set(ds18b20, Pin_OFF);
	
	Delay_us(480);
	Pin_Set(ds18b20, Pin_ON);
	
	Delay_us(15);
	
	setInput(ds18b20);
	
	uint16_t pluse_time = 0;
	
	while ( Pin_Get(ds18b20) && pluse_time < 60 )
	{
		Delay_us(1);// 1.5
		pluse_time ++;
	}
	
	// time out
	if ( pluse_time >= 60 )
		return 0;
	
	pluse_time = 0;
	
	while ( !Pin_Get(ds18b20) && pluse_time < 240 )
	{
		Delay_us(1);
		pluse_time ++;
	}
	
	// time out
	if ( pluse_time >= 240 )
		return 0;
	
	return 1;
}

static void ds18b20Write(uint8_t data)
{
	setOutput(ds18b20);
	
	for ( int i = 0; i < 8; i ++ )
	{
		Pin_Set(ds18b20, Pin_OFF);
		Delay_us(10);
		
		if ( data & 0x01 )	Pin_Set(ds18b20, Pin_ON);
		else				Pin_Set(ds18b20, Pin_OFF);
		
		Delay_us(20);
		
		Pin_Set(ds18b20, Pin_ON);
		
		//Delay_us(2);///
		
		data >>= 1;
	}
}

static uint8_t ds18b20Read(void)
{
	uint8_t data;
	
	for ( int i = 0; i < 8; i ++ )
	{
		data >>= 1;
		setOutput(ds18b20);
		Pin_Set(ds18b20, Pin_OFF);
		Delay_us(1);
		Pin_Set(ds18b20, Pin_ON);
		
		setInput(ds18b20);
		Delay_us(10);
		if ( Pin_Get(ds18b20) )	data |= 0x80;
		Delay_us(30);
	}
	
	return data;
}

static float ds18b20Temper(void)
{
	uint8_t d1, d2;
	if ( ds18b20Reset() == 0 )	return -100;
	ds18b20Write(0xCC);
	ds18b20Write(0x44);
	if ( !ds18b20Reset() )	return -100;
	ds18b20Write(0xCC);
	ds18b20Write(0xBE);
	
	d1 = ds18b20Read();
	d2 = ds18b20Read();
	return ((d2 << 8) | d1) * 0.0625f;
}

temper_t Temper18b20 = {
	ds18b20Temper
};