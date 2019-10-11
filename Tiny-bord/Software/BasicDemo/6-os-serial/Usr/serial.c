#include "serial.h"


/* private */
typedef struct
{
  __IO uint32_t ISR;   /*!< DMA interrupt status register */
  __IO uint32_t Reserved0;
  __IO uint32_t IFCR;  /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;












SerialDev * m_serialDev;



/*
	about --> printf
	uart && dma
*/

void emHAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	
	DMA_HandleTypeDef *hdma = huart->hdmatx;
	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);

	hdma->Instance->NDTR = Size;
	hdma->Instance->PAR = (uint32_t)&huart->Instance->DR;
	hdma->Instance->M0AR = *(uint32_t *)((uint32_t *)&pData);
	regs->IFCR = 0x3FU << hdma->StreamIndex;
	
	__HAL_DMA_ENABLE(hdma);
	__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);
	SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);
}

void em_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

	static uint8_t busy = 0;
	uint16_t length;
	
	while ( busy==1 && READ_BIT(m_serialDev->puart->Instance->SR, USART_SR_TC) == 0  )
	{
		//HAL_Delay(1);
	}
	busy = 1;

    length = vsnprintf(m_serialDev->tbuf, m_serialDev->tbufSize, (char*)format, args);
    va_end(args);
    //HAL_UART_Transmit(&huart1, (uint8_t*)printf_buf, length, 0xFFFFFFFF);
	emHAL_UART_Transmit_DMA(m_serialDev->puart, (uint8_t*)(m_serialDev->tbuf), length);
}



/*
	about --> receive
	IDLE(ISR) + DMA
	init buf -> 512 bytes
*/
int initSerial(SerialDev * serial)
{
	DMA_HandleTypeDef * hdma = serial->puart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
    DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
  
	/* Clear DBM bit */
	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
    hdma->Instance->NDTR = serial->rbufSize;

    hdma->Instance->PAR = (uint32_t)(&(serial->puart->Instance->DR));
    hdma->Instance->M0AR = (uint32_t)(serial->rbuf);
	
	regs->IFCR = 0x3FU << hdma->StreamIndex;
	//hdma->Instance->CR  |= DMA_IT_TC;
	
    __HAL_DMA_ENABLE(hdma);
	__HAL_UART_CLEAR_OREFLAG(serial->puart);
	
    SET_BIT(serial->puart->Instance->CR1, USART_CR1_IDLEIE);
	SET_BIT(serial->puart->Instance->CR3, USART_CR3_DMAR);	// enable DMA Receive
	return 0;
}
void restartDMA(SerialDev * serial)
{
	DMA_HandleTypeDef * hdma = serial->puart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
    DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
  
	/* Clear DBM bit */
	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
    hdma->Instance->NDTR = serial->rbufSize;

    hdma->Instance->PAR = (uint32_t)&serial->puart->Instance->DR;
    hdma->Instance->M0AR = (uint32_t)(serial->rbuf);
	
	regs->IFCR = 0x3FU << hdma->StreamIndex;
	//hdma->Instance->CR  |= DMA_IT_TC;
	__HAL_DMA_ENABLE(hdma);
}


int recCnt = 0;
void USART1_IRQHandler(void)
{
	__disable_irq();

	m_serialDev->rCnt = m_serialDev->rbufSize - READ_REG(m_serialDev->puart->hdmarx->Instance->NDTR);
	m_serialDev->rbuf[m_serialDev->rCnt] = '\0';

	em_printf("****---****\n");
	em_printf("ISR>>> %s\n", m_serialDev->rbuf);
    m_serialDev->RestartDma(m_serialDev);
	

	osSemaphoreRelease(*(m_serialDev->pSemSerial));
	
	m_serialDev->puart->Instance->DR;
	READ_REG(m_serialDev->puart->Instance->SR);
    __enable_irq();
	

}

SerialDev * registerSerial(UART_HandleTypeDef * huart, uint16_t rbufsize, uint16_t tbufsize, osSemaphoreId_t * pSemSerial)
{
	SerialDev * m_serial = (SerialDev*)malloc(sizeof(SerialDev));
    m_serial->puart = huart;
	
	
	m_serial->rbufSize   = rbufsize;
	m_serial->rbuf		 = NULL;
	m_serial->rbuf       = (char *)malloc(sizeof(char)*rbufsize);
	
	m_serial->tbufSize   = tbufsize;
	m_serial->tbuf		 = NULL;
	m_serial->tbuf	     = (char *)malloc(sizeof(char)*tbufsize);
	
	m_serial->pSemSerial = pSemSerial;

	m_serial->RestartDma = &restartDMA;
    m_serial->Initialize = &initSerial;
	
	
	//em_printf("Malloc m_serial: %d\n", malloc_usable_size(serial));
	
	/**/
	m_serialDev = m_serial;
    em_printf("Test!!!\n");
	/*Debug*/
	if (m_serial->rbuf == NULL )
	{
		em_printf("Malloc faild.\n");
	}
	else
	{
		em_printf("Malloc success.\n");
	}
	if (m_serial->tbuf == NULL )
	{
		em_printf("Malloc faild.\n");
	}
	else
	{
		em_printf("Malloc success.\n");
	}
    return m_serial;
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
