#include "startup.h"


void $Super$$main(void);


void $Sub$$main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM10_Init();
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	MX_USART2_UART_Init();
	MX_SPI2_Init();
	MX_CRC_Init();
	
	/**/
	MallocInit();
	OLED_Init();
	OLED_CLS();
	//OLED_Fill(0xFF);
	//OLED_DrawLine(0, 0, 127, 63, 1);
	//OLED_DrawLine(0, 63, 127, 0, 1);
	OLED_Replot();
	
	
	
	
	/**/
	$Super$$main();
}
