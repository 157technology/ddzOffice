#include "serial.h"

#include "usart.h"

uart_map_t uart_map;

/*##########*/
//device
static Serial *pserial1 = NULL;
static UART_HandleTypeDef *puart1 = NULL;
static Serial *pserial6 = NULL;
static UART_HandleTypeDef *puart6;

void registerUartDev()
{
	uart_map.insert(uart_map_t::value_type("com1", &huart1));
	uart_map.insert(uart_map_t::value_type("com6", &huart6));
}

/* private for hal register */
typedef struct
{
	__IO uint32_t ISR; /*!< DMA interrupt status register */
	__IO uint32_t Reserved0;
	__IO uint32_t IFCR; /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;

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

	while (busy == 1 && READ_BIT(puart1->Instance->SR, USART_SR_TC) == 0)
	{
	}
	busy = 1;

	// length = vsnprintf(tbuf, tbufSize, (char *)format, args);
	// va_end(args);
	// emHAL_UART_Transmit_DMA(puart1, (uint8_t *)(tbuf), length);
}

extern "C"
{
	void CONSOLE_ISR(void)
	{
		//EvrMyC1_Active(222);
		__disable_irq();
		//m_serialDev->puart->Instance->DR;
		READ_REG(puart1->Instance->SR);

		pserial1->isr_handle(READ_REG(puart1->hdmarx->Instance->NDTR));

		/* clear idle flag */
		puart1->Instance->DR;
		READ_REG(puart1->Instance->SR);

		__enable_irq();
		//EvrMyC1_Idle(222);
	}

	void WIFI_ISR(void)
	{
		__disable_irq();
		//m_serialDev->puart->Instance->DR;
		READ_REG(puart6->Instance->SR);

		pserial6->isr_handle(READ_REG(puart6->hdmarx->Instance->NDTR));

		/* clear idle flag */
		puart6->Instance->DR;
		READ_REG(puart6->Instance->SR);

		__enable_irq();
	}
}

/*###############################################################*/

Serial::Serial(string com)
{
	printf("Constructor Of Serial--string..\n");
	rbufSize = 512;
	rbuf = (char *)MALLOC(sizeof(char) * rbufSize);
	rCnt = 0;

	tbufSize = 512;
	tbuf = (char *)MALLOC(sizeof(char) * tbufSize);

	if (rbuf == NULL || tbuf == NULL)
	{
		printf("Malloc memory for buffer failed.\n");
	}
	auto iter = uart_map.find(com);
	m_com = com;
	if (iter->second == &huart1)
	{
		printf("Yeee, %s is exsit.\n", com.c_str());
		pserial1 = this;
		puart1 = &huart1;
	}
	else if (iter->second == &huart6)
	{
		printf("Yeee, %s is exsit.\n", com.c_str());
		pserial6 = this;
		puart6 = &huart6;
	}
}

Serial::~Serial()
{
	printf("~~~\n");
}

bool Serial::init()
{
	/*依据模块号与底层进行链接*/

	printf("Uart Select\n");

	/**/
	printf("Config OutPut\n");

	/**/
	printf("Config Input\n");
	UART_HandleTypeDef *puart;
	if (m_com == "com1")
		puart = puart1;
	else if (m_com == "com6")
		puart = puart6;
	DMA_HandleTypeDef *hdma = puart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

	/* Clear DBM bit */
	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
	hdma->Instance->NDTR = rbufSize;

	hdma->Instance->PAR = (uint32_t)(&puart->Instance->DR);
	hdma->Instance->M0AR = (uint32_t)(rbuf);

	regs->IFCR = 0x3FU << hdma->StreamIndex;

	__HAL_DMA_ENABLE(hdma);
	__HAL_UART_CLEAR_OREFLAG(puart);

	SET_BIT(puart->Instance->CR1, USART_CR1_IDLEIE);
	SET_BIT(puart->Instance->CR3, USART_CR3_DMAR); // enable DMA Receive

	if (m_com == "com1")
	{
		HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
	else if (m_com == "com6")
	{
		HAL_NVIC_SetPriority(USART6_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
	}

	return true;
}

bool Serial::open()
{
	/**/
	printf("Set open mode--Readonly|ReadWrite|WriteOnly.\n");

	return true;
}

void Serial::print(const char *format, ...)
{
	UART_HandleTypeDef *puart;
	if (m_com == "com1")
		puart = puart1;
	else if (m_com == "com6")
		puart = puart6;

	va_list args;
	va_start(args, format);

	static uint8_t busy = 0;
	uint16_t length;

	while (busy == 1 && READ_BIT(puart->Instance->SR, USART_SR_TC) == 0)
	{
	}
	busy = 1;

	length = vsnprintf(tbuf, tbufSize, (char *)format, args);
	va_end(args);
	emHAL_UART_Transmit_DMA(puart, (uint8_t *)(tbuf), length);
}

void Serial::readyRead(char *ch) {}
void Serial::emit_readyRead(char *data)
{
	emitWithParam(readyRead, (data), (data));
}

void Serial::isr_handle(int ndtr)
{
	rCnt = rbufSize - ndtr;
	rbuf[rCnt] = '\0';
	//emit signal

	emit_readyRead(rbuf);

	UART_HandleTypeDef *puart;
	if (m_com == "com1")
		puart = puart1;
	else if (m_com == "com6")
		puart = puart6;
	//reset dma
	DMA_HandleTypeDef *hdma = puart->hdmarx;

	__HAL_DMA_DISABLE(hdma);
	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

	/* Clear DBM bit */
	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
	hdma->Instance->NDTR = rbufSize;

	hdma->Instance->PAR = (uint32_t)&puart->Instance->DR;
	hdma->Instance->M0AR = (uint32_t)(rbuf);

	regs->IFCR = 0x3FU << hdma->StreamIndex;
	__HAL_DMA_ENABLE(hdma);
}