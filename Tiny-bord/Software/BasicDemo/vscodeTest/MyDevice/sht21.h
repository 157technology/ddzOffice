#ifndef __SHT21_HPP__
#define __SHT21_HPP__ 

#include "main.h"
#include "i2c.h"
#ifdef __cplusplus
extern "C" {
#endif
	

class SHT21
{
public:
	SHT21() {}
	~SHT21() {}
	
	float temper();
	float humidity();
};

#define SHT21_ADDRESS		0x81
#define SHT21_Temper_ADDR	0xE3
#define SHT21_Humidity_ADDR 0xE5

#define SHT21_IIC 			&hi2c1

#define __getTemper(num)	HAL_I2C_Mem_Read(SHT21_IIC, SHT21_ADDRESS, SHT21_Temper_ADDR, I2C_MEMADD_SIZE_8BIT, num, 2, 0xFFFF)
#define __getHumidity(num)	HAL_I2C_Mem_Read(SHT21_IIC, SHT21_ADDRESS, SHT21_Humidity_ADDR, I2C_MEMADD_SIZE_8BIT, num, 2, 0xFFFF)


#ifdef __cplusplus
}
#endif

#endif