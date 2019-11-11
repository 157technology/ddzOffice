#include "serial.h"


/* private for hal register */
typedef struct
{
  __IO uint32_t ISR;   /*!< DMA interrupt status register */
  __IO uint32_t Reserved0;
  __IO uint32_t IFCR;  /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;


static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
  /* Clear DBM bit */
  hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);

  /* Configure DMA Stream data length */
  hdma->Instance->NDTR = DataLength;

  /* Memory to Peripheral */
  if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
  {
    /* Configure DMA Stream destination address */
    hdma->Instance->PAR = DstAddress;

    /* Configure DMA Stream source address */
    hdma->Instance->M0AR = SrcAddress;
  }
  /* Peripheral to Memory */
  else
  {
    /* Configure DMA Stream source address */
    hdma->Instance->PAR = SrcAddress;

    /* Configure DMA Stream destination address */
    hdma->Instance->M0AR = DstAddress;
  }
}



/* serial device */
static SerialDev_t * m_consoleDev;



/*
	about --> printf
	uart && dma
*/

void emHAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
//	DMA_HandleTypeDef *hdma = huart->hdmatx;
//	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

//	DMA_SetConfig(hdma, (uint32_t)pData, (uint32_t)&huart->Instance->DR, Size);
//	regs->IFCR = 0x3FU << hdma->StreamIndex;
//	
//	__HAL_DMA_ENABLE(hdma);
//	__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);
//	SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);
	
	HAL_UART_Transmit_DMA(huart, pData, Size);
}

void em_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

	static uint8_t busy = 0;
	uint16_t length;
	
	while ( busy==1 && READ_BIT(m_consoleDev->puart->Instance->SR, USART_SR_TC) == 0  )
	{}
	busy = 1;

    length = vsnprintf(m_consoleDev->tbuf, m_consoleDev->tbufSize, (char*)format, args);
    va_end(args);
	emHAL_UART_Transmit_DMA(m_consoleDev->puart, (uint8_t*)(m_consoleDev->tbuf), length);
	//HAL_UART_Transmit(m_consoleDev->puart, (uint8_t*)(m_consoleDev->tbuf), length, 999);
}



/*
	about --> receive
	IDLE(ISR) + DMA
	init buf -> 512 bytes
*/
void initSerial(SerialDev_t * this)
{
	DMA_HandleTypeDef * hdma = this->puart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
    DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
  
	/* Clear DBM bit */
	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
    hdma->Instance->NDTR = this->rbufSize;

    hdma->Instance->PAR = (uint32_t)(&(this->puart->Instance->DR));
    hdma->Instance->M0AR = (uint32_t)(this->rbuf);
	
	regs->IFCR = 0x3FU << hdma->StreamIndex;
	
    __HAL_DMA_ENABLE(hdma);
	__HAL_UART_CLEAR_OREFLAG(this->puart);
	
    SET_BIT(this->puart->Instance->CR1, USART_CR1_IDLEIE);
	SET_BIT(this->puart->Instance->CR3, USART_CR3_DMAR);	// enable DMA Receive
}
void restartDMA(SerialDev_t * this)
{
	DMA_HandleTypeDef * hdma = this->puart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
    DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
  
	/* Clear DBM bit */
	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
    hdma->Instance->NDTR = this->rbufSize;

    hdma->Instance->PAR = (uint32_t)&this->puart->Instance->DR;
    hdma->Instance->M0AR = (uint32_t)(this->rbuf);
	
	regs->IFCR = 0x3FU << hdma->StreamIndex;
	__HAL_DMA_ENABLE(hdma);
}



//void SHELL_ISR(void)
//{
//	__disable_irq();
//	//m_serialDev->puart->Instance->DR;
//	READ_REG(m_consoleDev->puart->Instance->SR);	
//	
//	m_consoleDev->rCnt = m_consoleDev->rbufSize - READ_REG(m_consoleDev->puart->hdmarx->Instance->NDTR);
//	m_consoleDev->rbuf[m_consoleDev->rCnt] = '\0';
//	
//	//em_printf("ISR>>> %s, %d\n", m_consoleDev->rbuf, m_consoleDev->rCnt);
//    m_consoleDev->RestartDma(m_consoleDev);
//	//osSemaphoreRelease(*(m_consoleDev->pSemSerial));
//	
//	/* clear idle flag */
//	m_consoleDev->puart->Instance->DR;
//	READ_REG(m_consoleDev->puart->Instance->SR);
//	
//    __enable_irq();
//}


SerialDev_t * registerSerial(UART_HandleTypeDef * huart, uint16_t rbufsize, uint16_t tbufsize, osSemaphoreId_t * pSemSerial)
{
	SerialDev_t * m_serial = (SerialDev_t*)EM_MALLOC(sizeof(SerialDev_t));
    m_serial->puart = huart;
	
	
	m_serial->rbufSize   = rbufsize;
	m_serial->rbuf		 = NULL;
	m_serial->rbuf       = (char *)EM_MALLOC(sizeof(char)*rbufsize);
	
	m_serial->tbufSize   = tbufsize;
	m_serial->tbuf		 = NULL;
	m_serial->tbuf	     = (char *)malloc(sizeof(char)*tbufsize);
	
	#ifdef USING_OS
	m_serial->pSemSerial = pSemSerial;
	#endif

	m_serial->RestartDma = &restartDMA;
    m_serial->Initialize = &initSerial;
 
	
	if ( huart == &SHELL_UART )
	{
		m_serial->print = em_printf;
		m_consoleDev = m_serial;
	}
	else
	{
		em_printf("Serial Device Not Found.\n");
	}

    return m_serial;
}