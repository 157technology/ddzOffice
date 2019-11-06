/************************************************************************************
*  Copyright (c), 2014, HelTec Automatic Technology co.,LTD.
*            All rights reserved.
*
* Http:    www.heltec.cn
* Email:   cn.heltec@gmail.com
* WebShop: heltec.taobao.com
*
* File name: OLED_I2C.c
* Project  : HelTec.uvprij
* Processor: STM32F103C8T6
* Compiler : MDK fo ARM
*
* Author : Ð¡ÁÖ
* Version: 1.00
* Date   : 2014.4.8
* Email  : hello14blog@gmail.com
* Modification: none
*
* Description:128*64µãÕóµÄOLEDÏÔÊ¾ÆÁÇý¶¯ÎÄ¼þ£¬½öÊÊÓÃÓÚ»ÝÌØ×Ô¶¯»¯(heltec.taobao.com)µÄSD1306Çý¶¯IICÍ¨ÐÅ·½Ê½ÏÔÊ¾ÆÁ
*
* Others: none;
*
* Function List:
*	1. void I2C_Configuration(void) -- ÅäÖÃCPUµÄÓ²¼þI2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- Ïò¼Ä´æÆ÷µØÖ·Ð´Ò»¸öbyteµÄÊý¾Ý
* 3. void WriteCmd(unsigned char I2C_Command) -- Ð´ÃüÁî
* 4. void WriteDat(unsigned char I2C_Data) -- Ð´Êý¾Ý
* 5. void OLED_Init(void) -- OLEDÆÁ³õÊ¼»¯
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- ÉèÖÃÆðÊ¼µã×ø±ê
* 7. void OLED_Fill(unsigned char fill_Data) -- È«ÆÁÌî³ä
* 8. void OLED_CLS(void) -- ÇåÆÁ
* 9. void OLED_ON(void) -- »½ÐÑ
* 10. void OLED_OFF(void) -- Ë¯Ãß
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- ÏÔÊ¾×Ö·û´®(×ÖÌå´óÐ¡ÓÐ6*8ºÍ8*16Á½ÖÖ)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- ÏÔÊ¾ÖÐÎÄ(ÖÐÎÄÐèÒªÏÈÈ¡Ä££¬È»ºó·Åµ½codetab.hÖÐ)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMPÍ¼Æ¬
*
* History: none;
*
*************************************************************************************/

#include "OLED_I2C.h"

#include "codetab.h"
#include "serial.h"
#include "memory.h"

static uint8_t (* buffer)[128];
static uint8_t (* vmBuffer)[128];
static uint8_t (* vmFlag)[128];
void WriteCmd(unsigned char I2C_Command)//Ð´ÃüÁî
{
	uint8_t *pData;

	pData = &I2C_Command;

	HAL_I2C_Mem_Write(&hi2c1, OLED_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, pData, 1, 100);
}

void WriteDat(unsigned char I2C_Data)//Ð´Êý¾Ý
{

	uint8_t *pData;

	pData = &I2C_Data;

	HAL_I2C_Mem_Write(&hi2c1, OLED_ADDRESS, 0x40, I2C_MEMADD_SIZE_8BIT, pData, 1, 100);
}

void OLED_Init(void)
{
	buffer   = MALLOC(128 * 8 * sizeof(char));
	vmBuffer = MALLOC(128 * 8 * sizeof(char));
	vmFlag   = MALLOC(128 * 8 * sizeof(char));
	memset((uint8_t*)vmFlag, 1, 128 * 8 * sizeof(char));


	HAL_Delay(100); //ÕâÀïµÄÑÓÊ±ºÜÖØÒª

	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //ÁÁ¶Èµ÷½Ú 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //ÉèÖÃÆðÊ¼µã×ø±ê
{
	WriteCmd(0xb0 + y);
	WriteCmd(((x & 0xf0) >> 4) | 0x10);
	WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Fill(unsigned char fill_Data)//È«ÆÁÌî³ä
{
	unsigned char m, n;
	for (m = 0; m < 8; m++)
	{
		WriteCmd(0xb0 + m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for (n = 0; n < 128; n++)
		{
			WriteDat(fill_Data);
		}
	}
}

void OLED_CLS(void)//ÇåÆÁ
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          :
// Parameters     : none
// Description    : ½«OLED´ÓÐÝÃßÖÐ»½ÐÑ
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //ÉèÖÃµçºÉ±Ã
	WriteCmd(0X14);  //¿ªÆôµçºÉ±Ã
	WriteCmd(0XAF);  //OLED»½ÐÑ
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          :
// Parameters     : none
// Description    : ÈÃOLEDÐÝÃß -- ÐÝÃßÄ£Ê½ÏÂ,OLED¹¦ºÄ²»µ½10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //ÉèÖÃµçºÉ±Ã
	WriteCmd(0X10);  //¹Ø±ÕµçºÉ±Ã
	WriteCmd(0XAE);  //OLEDÐÝÃß
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          :
// Parameters     : x,y -- ÆðÊ¼µã×ø±ê(x:0~127, y:0~7); ch[] -- ÒªÏÔÊ¾µÄ×Ö·û´®; TextSize -- ×Ö·û´óÐ¡(1:6*8 ; 2:8*16)
// Description    : ÏÔÊ¾codetab.hÖÐµÄASCII×Ö·û,ÓÐ6*8ºÍ8*16¿ÉÑ¡Ôñ
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0, i = 0, j = 0;
	switch (TextSize)
	{
	case 1:
	{
		while (ch[j] != '\0')
		{
			c = ch[j] - 32;
			if (x > 126)
			{
				x = 0;
				y++;
			}
			OLED_SetPos(x, y);
			for (i = 0; i < 6; i++)
				WriteDat(F6x8[c][i]);
			x += 6;
			j++;
		}
	} break;
	case 2:
	{
		while (ch[j] != '\0')
		{
			c = ch[j] - 32;
			if (x > 120)
			{
				x = 0;
				y++;
			}
			OLED_SetPos(x, y);
			for (i = 0; i < 8; i++)
				WriteDat(F8X16[c * 16 + i]);
			OLED_SetPos(x, y + 1);
			for (i = 0; i < 8; i++)
				WriteDat(F8X16[c * 16 + i + 8]);
			x += 8;
			j++;
		}
	} break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          :
// Parameters     : x,y -- ÆðÊ¼µã×ø±ê(x:0~127, y:0~7); N:ºº×ÖÔÚcodetab.hÖÐµÄË÷Òý
// Description    : ÏÔÊ¾codetab.hÖÐµÄºº×Ö,16*16µãÕó
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm = 0;
	unsigned int  adder = 32 * N;
	OLED_SetPos(x , y);
	for (wm = 0; wm < 16; wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x, y + 1);
	for (wm = 0; wm < 16; wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          :
// Parameters     : x0,y0 -- ÆðÊ¼µã×ø±ê(x0:0~127, y0:0~7); x1,y1 -- Æðµã¶Ô½ÇÏß(½áÊøµã)µÄ×ø±ê(x1:1~128,y1:1~8)
// Description    : ÏÔÊ¾BMPÎ»Í¼
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_SetPos(x0, y);
		for (x = x0; x < x1; x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

////////////////////////////////////////////////////////////
///
///
///
///
///
///////////////////////////////////////////////////////////

void OLED_BufToPanel(void)
{
	uint8_t i,j;
	uint8_t state = 0;

	uint16_t count = 0;

	for (i = 0 ; i < 8; i++)
	{
		WriteCmd (0xB0 + i);
		WriteCmd (0x00);
		WriteCmd (0x10);

		for (j = 0 ; j < 128; j++)
		{
			if ( vmFlag[i][j] == 1 )
			{
				if ( state == 0 )
				{
					//em_printf("Set pos(%d,%d).\n", i, j);
					WriteCmd (0xB0 + i);					/* 设置页地址 */
					WriteCmd (0x00 + (j & 0x0F));			/* 设置列地址的低地址 */
					WriteCmd (0x10 + ((j >> 4) & 0x0F));	/* 设置列地址的高地址 */
					state = 1;
				}

				count ++;
				WriteDat(buffer[i][j]);
				vmBuffer[i][j] = buffer[i][j];
				vmFlag[i][j] = 0;
			}
			else
			{
				state = 0;
			}
			
		}
	}

	//em_printf("Repoint %d times (ALL 1024).\n", count);
}


/*
	设置pos(x, y) 的颜色
	x: 0 - 127
	y: 0 - 63
 */
const uint8_t aOrTab[8]  = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
const uint8_t aAndTab[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
void OLED_SetPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;

	ucPageAddr = _usY / 8;
	ucColAddr = _usX;

	ucValue = buffer[ucPageAddr][ucColAddr];
	if (_ucColor == 0)
	{
		ucValue &= aAndTab[_usY % 8];
	}
	else
	{
		ucValue |= aOrTab[_usY % 8];
	}
	buffer[ucPageAddr][ucColAddr] = ucValue;
	if ( buffer[ucPageAddr][ucColAddr] != vmBuffer[ucPageAddr][ucColAddr] )
	{
		vmFlag[ucPageAddr][ucColAddr] = 1;
	}

	//if (s_ucUpdateEn == 1)
//	{
//		WriteCmd (0xB0 + ucPageAddr);					/* 设置页地址 */
//		WriteCmd (0x00 + (ucColAddr & 0x0F));			/* 设置列地址的低地址 */
//		WriteCmd (0x10 + ((ucColAddr >> 4) & 0x0F));	/* 设置列地址的高地址 */
//		WriteDat(ucValue);
//	}
}

/*
	读取pos(x, y)的像素值
 */
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;

	ucPageAddr = _usY / 8;
	ucColAddr = _usX;

	ucValue = buffer[ucPageAddr][ucColAddr];
	//if (ucValue & (_usY % 8))
	if ( ucValue & (0x01 << (_usY % 8)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* use Breseham algrithm */

	OLED_SetPixel(_usX1 , _usY1 , _ucColor);

	/* if this two point is the same*/
	if ( _usX1 == _usX2 && _usY1 == _usY2 )
	{
		return;
	}

	iTag = 0 ;
	/* dx = abs ( _usX2 - _usX1 ); */
	if (_usX2 >= _usX1)
	{
		dx = _usX2 - _usX1;
	}
	else
	{
		dx = _usX1 - _usX2;
	}

	/* dy = abs ( _usY2 - _usY1 ); */
	if (_usY2 >= _usY1)
	{
		dy = _usY2 - _usY1;
	}
	else
	{
		dy = _usY1 - _usY2;
	}

	if ( dx < dy )   /* 如多dy为较长方向, 则交换横纵坐标 */
	{
		uint16_t temp;

		iTag = 1 ;
		temp = _usX1; _usX1 = _usY1; _usY1 = temp;
		temp = _usX2; _usX2 = _usY2; _usY2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = _usX2 > _usX1 ? 1 : -1 ;    /* make sure add or sub */
	ty = _usY2 > _usY1 ? 1 : -1 ;
	x = _usX1 ;
	y = _usY1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != _usX2 )     /* Ñ­»·»­µã */
	{
		if ( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if ( iTag )
		{
			OLED_SetPixel ( y , x , _ucColor) ;
		}
		else
		{
			OLED_SetPixel ( x , y , _ucColor) ;
		}
		x += tx ;
	}
}

void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor)
{
	uint16_t i;

	for (i = 0 ; i < _usSize - 1; i++)
	{
		OLED_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _ucColor);
	}
}

void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor)
{
	/*
	 ---------------->---
	|(_usX£¬_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	OLED_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _ucColor);	/* ¶¥ */
	OLED_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _ucColor);	/* µ× */

	OLED_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _ucColor);	/* ×ó */
	OLED_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _ucColor);	/* ÓÒ */
}

void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* µ±Ç° X Öµ */
	uint32_t  CurY;		/* µ±Ç° Y Öµ */

	D = 3 - (_usRadius << 1);
	CurX = 0;
	CurY = _usRadius;

	while (CurX <= CurY)
	{
		OLED_SetPixel(_usX + CurX, _usY + CurY, _ucColor);
		OLED_SetPixel(_usX + CurX, _usY - CurY, _ucColor);
		OLED_SetPixel(_usX - CurX, _usY + CurY, _ucColor);
		OLED_SetPixel(_usX - CurX, _usY - CurY, _ucColor);
		OLED_SetPixel(_usX + CurY, _usY + CurX, _ucColor);
		OLED_SetPixel(_usX + CurY, _usY - CurX, _ucColor);
		OLED_SetPixel(_usX - CurY, _usY + CurX, _ucColor);
		OLED_SetPixel(_usX - CurY, _usY - CurX, _ucColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}