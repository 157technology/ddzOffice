#ifndef __OLED_CONFIG_H__
#define __OLED_CONFIG_H__ 

#include "main.h"
#include "i2c.h"

#define OLED_ADDRESS	0x78

#define OLED_IIC		&hi2c1

#define __WriteCmd(cmd)	HAL_I2C_Mem_Write(OLED_IIC, OLED_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &(cmd), 1, 100)
#define __WriteDat(dat)	HAL_I2C_Mem_Write(OLED_IIC, OLED_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, &(dat), 1, 100)


#endif