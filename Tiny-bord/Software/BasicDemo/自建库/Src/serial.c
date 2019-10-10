#include "serial.h"

uint8_t recvbuf[512];
SerialDev m_serialDev;
int initSerial(UART_HandleTypeDef * huart, uint16_t size)
{
	DMA_HandleTypeDef * hdma = huart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
    //DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
    hdma->Instance->NDTR = size;

    hdma->Instance->PAR = (uint32_t)huart->Instance->DR;
    hdma->Instance->M0AR = (uint32_t)recvbuf;
	
	__HAL_DMA_ENABLE_IT(hdma, DMA_IT_TC);
    __HAL_DMA_ENABLE(hdma);
	
	__HAL_UART_DISABLE_IT(huart, UART_IT_TC);
	
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);
    CLEAR_BIT(huart->Instance->SR, USART_SR_IDLE);    // clear idle interrupt
    SET_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
	
	return 0;
}

char printf_buf[512];
uint16_t length;
void em_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    //while ( READ_BIT(huart1.Instance->SR, USART_SR_TC) == 1)
    {
		HAL_Delay(20);
        //osDelay(1);
    }

    length = vsnprintf((char*)printf_buf, sizeof(printf_buf), (char*)format, args);
    va_end(args);
    HAL_UART_Transmit_DMA(&huart1, (uint8_t*)printf_buf, length);
}

SerialDev * registerSerial(UART_HandleTypeDef * huart, uint16_t size)
{
    m_serialDev.puart = huart;
    m_serialDev.size = 0;
    m_serialDev.revData  = (char *)recvbuf;

    m_serialDev.Initialize = &initSerial;
    
    return &m_serialDev;
}


//void USART1_IRQHandler(void)
//{
//  /* USER CODE BEGIN USART1_IRQn 0 */
//	if ( READ_BIT(huart1.Instance->SR, USART_SR_TC) == 1 )
//	{
//		CLEAR_BIT(huart1.Instance->SR, USART_SR_TC);
//		CLEAR_BIT(huart1.Instance->CR1, USART_CR1_TCIE);
//		return;
//	}
//  /* USER CODE END USART1_IRQn 0 */
//  //HAL_UART_IRQHandler(&huart1);
//  /* USER CODE BEGIN USART1_IRQn 1 */
//    uint16_t pos = 512 - ((DMA_Stream_TypeDef *)(huart1.hdmarx)->Instance)->NDTR;
//    __disable_irq();
//    CLEAR_BIT(huart1.Instance->SR, USART_SR_IDLE);
//    //serialDev.size = pos;
//    recvbuf[pos] = '\0';
//	em_printf("ISR>>>");
//    em_printf("ISR>>> size: %-3d string: %s.\n", pos, recvbuf);
//    //osSemaphoreRelease(Serial_Sem);
//    initSerial(&huart1, 512);

//    __enable_irq();
//  /* USER CODE END USART1_IRQn 1 */
//}

///**
//  * @brief This function handles DMA2 stream2 global interrupt.
//  */
//void DMA2_Stream2_IRQHandler(void)
//{
//  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

//  /* USER CODE END DMA2_Stream2_IRQn 0 */
//  //HAL_DMA_IRQHandler(&hdma_usart1_rx);
//  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

//  /* USER CODE END DMA2_Stream2_IRQn 1 */
//}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
