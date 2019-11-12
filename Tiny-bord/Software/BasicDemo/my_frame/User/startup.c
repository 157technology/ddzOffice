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
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	MX_USART2_UART_Init();
	MX_SPI2_Init();
	MX_CRC_Init();
	
	
	
	$Super$$main();
}
