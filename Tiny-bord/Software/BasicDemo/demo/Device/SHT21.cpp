#include "SHT21.hpp"
#include "SHT21_config.h"


float SHT21::temper()
{
	uint8_t T[2];
	uint16_t data;
	__getTemper(T);
	data = ((uint16_t)T[0] << 8) + (T[1] & 0xfe);
	
	return data*175.72/65536 - 46.85;
}

float SHT21::humidity()
{
	uint8_t H[2];
	uint16_t data;
	__getHumidity(H);
	data = ((uint16_t)H[0] << 8) + (H[1] & 0xfe);
	return data*125.00/65536 - 6.00;
}