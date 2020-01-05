#include "serial.h"

#include "usart.h"

/*##########*/

/* private for hal register */
typedef struct
{
	__IO uint32_t ISR; /*!< DMA interrupt status register */
	__IO uint32_t Reserved0;
	__IO uint32_t IFCR; /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;

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
/*
	about --> printf
	uart && dma
*/
//device
void ReadyRead(void *data){};
void WifiRead(void *data) {}
static Serial *m_com1;
static Serial *m_com6;
Serial *Serial_Regester(UART_HandleTypeDef *puart, int rbufSize, int tbufSize)
{
	Serial *ps = (Serial *)MALLOC(sizeof(Serial));
	ps->puart = puart;
	ps->rbufSize = rbufSize;
	ps->tbufSize = tbufSize;
	ps->rCnt = 0;
	ps->rbuf = (char *)MALLOC(sizeof(char) * rbufSize);
	ps->tbuf = (char *)MALLOC(sizeof(char) * tbufSize);
	return ps;
}

void Serial_Init(Serial *ps)
{
	UART_HandleTypeDef *puart = ps->puart;
	DMA_HandleTypeDef *hdma = puart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

	/* Clear DBM bit */
	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
	hdma->Instance->NDTR = ps->rbufSize;

	hdma->Instance->PAR = (uint32_t)(&puart->Instance->DR);
	hdma->Instance->M0AR = (uint32_t)(ps->rbuf);

	regs->IFCR = 0x3FU << hdma->StreamIndex;

	__HAL_DMA_ENABLE(hdma);
	__HAL_UART_CLEAR_OREFLAG(puart);

	SET_BIT(puart->Instance->CR1, USART_CR1_IDLEIE);
	SET_BIT(puart->Instance->CR3, USART_CR3_DMAR); // enable DMA Receive

	if (puart == &huart1)
	{
		ps->signal = ReadyRead;
		m_com1 = ps;
		HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
	else if (puart == &huart6)
	{
		ps->signal = WifiRead;
		m_com6 = ps;
		HAL_NVIC_SetPriority(USART6_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
	}
}



void Serial_Print(Serial *ps, const char *format, ...)
{
	UART_HandleTypeDef *puart = ps->puart;
	va_list args;
	va_start(args, format);

	static uint8_t busy = 0;
	uint16_t length;

	while (busy == 1 && READ_BIT(puart->Instance->SR, USART_SR_TC) == 0)
	{
	}
	busy = 1;

	length = vsnprintf(ps->tbuf, ps->tbufSize, (char *)format, args);
	va_end(args);
	emHAL_UART_Transmit_DMA(puart, (uint8_t *)(ps->tbuf), length);
}

inline static void isr_handle(Serial *ps, int ndtr)
{
	ps->rCnt = ps->rbufSize - ndtr;
	ps->rbuf[ps->rCnt] = '\0';
	//emit signal

	//emit_readyRead(rbuf);
	emit(ps->signal, (void *)ps->rbuf);

	UART_HandleTypeDef *puart = ps->puart;
	//reset dma
	DMA_HandleTypeDef *hdma = puart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

	/* Clear DBM bit */
	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
	hdma->Instance->NDTR = ps->rbufSize;

	hdma->Instance->PAR = (uint32_t)&puart->Instance->DR;
	hdma->Instance->M0AR = (uint32_t)(ps->rbuf);

	regs->IFCR = 0x3FU << hdma->StreamIndex;
	__HAL_DMA_ENABLE(hdma);
}

void em_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	static uint8_t busy = 0;
	uint16_t length;

	while (busy == 1 && READ_BIT(huart1.Instance->SR, USART_SR_TC) == 0)
	{
	}
	busy = 1;

	length = vsnprintf(m_com1->tbuf, m_com1->tbufSize, (char *)format, args);
	va_end(args);
	emHAL_UART_Transmit_DMA(m_com1->puart, (uint8_t *)(m_com1->tbuf), length);
}

void CONSOLE_ISR(void)
{
	//EvrMyC1_Active(222);
	__disable_irq();
	//m_serialDev->puart->Instance->DR;
	READ_REG(m_com1->puart->Instance->SR);

	isr_handle(m_com1, READ_REG(m_com1->puart->hdmarx->Instance->NDTR));

	/* clear idle flag */
	m_com1->puart->Instance->DR;
	READ_REG(m_com1->puart->Instance->SR);

	__enable_irq();
	//EvrMyC1_Idle(222);
}

void WIFI_ISR(void)
{
	__disable_irq();
	//m_serialDev->puart->Instance->DR;
	READ_REG(m_com6->puart->Instance->SR);

	isr_handle(m_com6, READ_REG(m_com6->puart->hdmarx->Instance->NDTR));

	/* clear idle flag */
	m_com6->puart->Instance->DR;
	READ_REG(m_com6->puart->Instance->SR);

	__enable_irq();
}

/*###############################################################*/
