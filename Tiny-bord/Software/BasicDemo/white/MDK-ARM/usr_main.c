#include "main.h"

#include "gpio.h"

int main()
{
	while ( 1 )
	{
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		HAL_Delay(200);
	}
	
	return 0;
}