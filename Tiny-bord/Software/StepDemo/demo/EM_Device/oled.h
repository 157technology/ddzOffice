#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "main.h"
#include "i2c.h"
//


#ifdef __cplusplus
extern "C" {
#endif



#define OLED_ADDRESS	0x78
#define OLED_IIC		&hi2c1
#define __WriteCmd(cmd)	HAL_I2C_Mem_Write(OLED_IIC, OLED_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &(cmd), 1, 0xffff)
#define __WriteDat(dat)	HAL_I2C_Mem_Write_DMA(OLED_IIC, OLED_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, &(dat), 1)



void OLED_Init(void);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);

void OLED_Replot(void);
void OLED_BufToPanel(void);
void OLED_SetPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor);
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY);
void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor);
void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor);
void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor);
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor);

	
	
#ifdef __cplusplus
}
#endif
	
#endif
