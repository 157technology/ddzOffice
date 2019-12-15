#include "common.hpp"

//#include "SEGGER_SYSVIEW.h"
#include "EventRecorder.h"              // Keil.ARM Compiler::Compiler:Event Recorder

void $Super$$main(void);

void $Sub$$main(void)
{
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */
	EventRecorderInitialize (EventRecordAll, 1);
	EventRecorderStart();
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	MX_USART6_UART_Init();
	MX_SPI2_Init();
	MX_TIM2_Init();
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	//SEGGER_SYSVIEW_Conf();
	
	$Super$$main();
}