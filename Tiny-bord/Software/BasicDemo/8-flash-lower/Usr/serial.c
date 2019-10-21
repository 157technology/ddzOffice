#include "serial.h"


//#include <stdio.h>
//struct __FILE
//{
//  int handle;
//  /* Whatever you require here. If the only file you are using is */
//  /* standard output using printf() for debugging, no file handling */
//  /* is required. */
//};
///* FILE is typedef’d in stdio.h. */
//FILE __stdout;
//int fputc(int ch, FILE *f) 
//{
//	return HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 999);
//}
//int ferror(FILE *f)
//{
//  /* Your implementation of ferror(). */
//  return 0;
//}


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
	{}
	busy = 1;

    length = vsnprintf(m_serialDev->tbuf, m_serialDev->tbufSize, (char*)format, args);
    va_end(args);
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
	__HAL_DMA_ENABLE(hdma);
}


int recCnt = 0;
void USART1_IRQHandler(void)
{
	__disable_irq();
	//m_serialDev->puart->Instance->DR;
	READ_REG(m_serialDev->puart->Instance->SR);	
	
	m_serialDev->rCnt = m_serialDev->rbufSize - READ_REG(m_serialDev->puart->hdmarx->Instance->NDTR);
	m_serialDev->rbuf[m_serialDev->rCnt] = '\0';
	
	//em_printf("ISR>>> %s, %d\n", m_serialDev->rbuf, m_serialDev->rCnt);
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
	
	m_serialDev = m_serial;

    return m_serial;
}