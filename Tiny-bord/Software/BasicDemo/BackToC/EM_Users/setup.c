#include "commom.h"
#include "EventRecorder.h"

void $Super$$main(void);
 
void $Sub$$main(void)
{
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_I2C1_Init();
    MX_USART6_UART_Init();
    MX_SPI2_Init();
    MX_TIM2_Init();

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
    /*###################################*/
    EventRecorderInitialize(EventRecordAll, 1);
    MallocInit(); //初始化动态内存

    OLED_Init();
	OLED_Fill(0x00);
    $Super$$main();
}