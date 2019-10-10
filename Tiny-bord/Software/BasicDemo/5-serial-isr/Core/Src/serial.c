#include "serial.h"

int rex_recv = 0;

typedef struct
{
  __IO uint32_t ISR;   /*!< DMA interrupt status register */
  __IO uint32_t Reserved0;
  __IO uint32_t IFCR;  /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;

/*
	about --> printf
	uart && dma
*/
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
    //HAL_UART_Transmit(&huart1, (uint8_t*)printf_buf, length, 0xFFFFFFFF);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t*)printf_buf, length);
}



/*
	about --> receive
	IDLE(ISR) + DMA
	init buf -> 512 bytes
*/
uint8_t recvbuf[512];

int initSerial(UART_HandleTypeDef * huart, uint16_t size)
{
	DMA_HandleTypeDef * hdma = huart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
    DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
  
	/* Clear DBM bit */
	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
    hdma->Instance->NDTR = size;

    hdma->Instance->PAR = (uint32_t)&huart->Instance->DR;
    hdma->Instance->M0AR = (uint32_t)(&recvbuf);
	
	regs->IFCR = 0x3FU << hdma->StreamIndex;
	hdma->Instance->CR  |= DMA_IT_TC;
	
    __HAL_DMA_ENABLE(hdma);
	__HAL_UART_CLEAR_OREFLAG(huart);
	
    SET_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
	SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);	// enable DMA Receive
	return 0;
}

int emUART_Transmit_DMA()
{
	return 0;
}

int recCnt = 0;
void USART1_IRQHandler(void)
{
	__disable_irq();
    READ_REG(huart1.Instance->SR);
    recCnt = 512 - READ_REG(huart1.hdmarx->Instance->NDTR);
	recvbuf[recCnt] = '\0';
	//em_printf("ISR>>> %s\n", recvbuf);
	rex_recv = 1;
    initSerial(&huart1, 512);

    //osSemaphoreRelease(serialSem_id);
    __enable_irq();

}
/*
SerialDev * registerSerial(UART_HandleTypeDef * huart, uint16_t size)
{
    m_serialDev.puart = huart;
    m_serialDev.size = 0;
    m_serialDev.revData  = (char *)recvbuf;

    m_serialDev.Initialize = &initSerial;
    
    return &m_serialDev;
}
*/

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
