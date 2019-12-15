#include "serial.h"

#define MyEvtC_DEBUG
#include "MyEventComponent.h"


uart_map_t uart_map;

void registerUartDev()
{
	uart_map.insert(uart_map_t::value_type("com1", &huart1));
	uart_map.insert(uart_map_t::value_type("com6", &huart6));
}

/* private for hal register */
typedef struct
{
	__IO uint32_t ISR;   /*!< DMA interrupt status register */
	__IO uint32_t Reserved0;
	__IO uint32_t IFCR;  /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;



Serial * pserial = NULL;
UART_HandleTypeDef * puart = NULL;
/* serial device */
//static SerialDev_t * m_consoleDev;



void hasData(void * data) {};

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

	while ( busy == 1 && READ_BIT( puart->Instance->SR, USART_SR_TC) == 0  )
	{}
	busy = 1;

//	length = vsnprintf( tbuf,  tbufSize, (char*)format, args);
//	va_end(args);
//	emHAL_UART_Transmit_DMA( puart, (uint8_t*)( tbuf), length);
}



/*
	about --> receive
	IDLE(ISR) + DMA
	init buf -> 512 bytes
*/
// void initSerial(SerialDev_t * my)
// {
// 	DMA_HandleTypeDef * hdma = my->puart->hdmarx;

// 	__HAL_DMA_DISABLE(hdma);
// 	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

// 	/* Clear DBM bit */
// 	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
// 	hdma->Instance->NDTR = my->rbufSize;

// 	hdma->Instance->PAR = (uint32_t)(&(my->puart->Instance->DR));
// 	hdma->Instance->M0AR = (uint32_t)(my->rbuf);

// 	regs->IFCR = 0x3FU << hdma->StreamIndex;

// 	__HAL_DMA_ENABLE(hdma);
// 	__HAL_UART_CLEAR_OREFLAG(my->puart);

// 	SET_BIT(my->puart->Instance->CR1, USART_CR1_IDLEIE);
// 	SET_BIT(my->puart->Instance->CR3, USART_CR3_DMAR);	// enable DMA Receive

// 	HAL_NVIC_SetPriority(USART1_IRQn, 1 , 0);
// 	HAL_NVIC_EnableIRQ(USART1_IRQn);
// }
// void restartDMA(SerialDev_t * my)
// {
// 	DMA_HandleTypeDef * hdma = my->puart->hdmarx;

// 	__HAL_DMA_DISABLE(hdma);
// 	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

// 	/* Clear DBM bit */
// 	hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);
// 	hdma->Instance->NDTR = my->rbufSize;

// 	hdma->Instance->PAR = (uint32_t)&my->puart->Instance->DR;
// 	hdma->Instance->M0AR = (uint32_t)(my->rbuf);

// 	regs->IFCR = 0x3FU << hdma->StreamIndex;
// 	__HAL_DMA_ENABLE(hdma);
// }


extern "C" {
	void CONSOLE_ISR(void)
	{
		EvrMyC1_Active(222);
		__disable_irq();
		//m_serialDev->puart->Instance->DR;
		READ_REG( puart->Instance->SR);

		//pserial->rCnt =  pserial->rbufSize - READ_REG( puart->hdmarx->Instance->NDTR);
		//rbuf[ rCnt] = '\0';

		//em_printf("ISR>>> %s, %d\n",  rbuf,  rCnt);
		//em_printf("ISR>>> %s, %d\n",  rbuf,  rCnt);
		//emit(hasData,  rbuf);
		//if ( pserial != NULL )
		pserial->isr_handle(READ_REG(puart->hdmarx->Instance->NDTR));
		//RestartDma(m_consoleDev);
		//osSemaphoreRelease(*( pSemSerial));


		/* clear idle flag */
		puart->Instance->DR;
		READ_REG( puart->Instance->SR);

		__enable_irq();
		EvrMyC1_Idle(222);
	}
}

// SerialDev_t * registerSerial(UART_HandleTypeDef * huart, uint16_t rbufsize, uint16_t tbufsize, osSemaphoreId_t * pSemSerial)
// {
// 	SerialDev_t * m_serial = (SerialDev_t*)malloc(sizeof(SerialDev_t));
// 	m_serial->puart = huart;


// 	m_serial->rbufSize   = rbufsize;
// 	m_serial->rbuf		 = NULL;
// 	m_serial->rbuf       = (char *)MALLOC(sizeof(char) * rbufsize);

// 	m_serial->tbufSize   = tbufsize;
// 	m_serial->tbuf		 = NULL;
// 	m_serial->tbuf	     = (char *)MALLOC(sizeof(char) * tbufsize);

// #ifdef USING_OS
// 	m_serial->pSemSerial = pSemSerial;
// #endif

// 	m_serial->RestartDma = &restartDMA;
// 	m_serial->Initialize = &initSerial;


// 	if ( huart == &CONSOLE_UART )
// 	{
// 		m_serial->print = em_printf;
// 		m_consoleDev = m_serial;
// 	}
// 	else
// 	{
// 		em_printf("Serial Device Not Found.\n");
// 	}

// 	return m_serial;
// }

/*###############################################################*/
Serial::Serial()
{
	printf("Constructor Of Serial..\n");

	/* set buffer */
	rbufSize = 512;
	rbuf       = (char *)MALLOC(sizeof(char) * rbufSize);
	rCnt = 0;

	tbufSize = 512;
	tbuf = (char *)MALLOC(sizeof(char) * tbufSize);

	if ( rbuf == NULL || tbuf == NULL )
	{
		printf("Malloc memory for buffer failed.\n");
	}
	pserial = this;
}

Serial::Serial(string com)
{
	printf("Constructor Of Serial--string..\n");
	rbufSize = 512;
	rbuf       = (char *)MALLOC(sizeof(char) * rbufSize);
	rCnt = 0;

	tbufSize = 512;
	tbuf = (char *)MALLOC(sizeof(char) * tbufSize);

	if ( rbuf == NULL || tbuf == NULL )
	{
		printf("Malloc memory for buffer failed.\n");
	}
	auto iter = uart_map.find(com);
	if ( iter->second == &huart1 )
	{
		printf("Yeee, %s is exsit.\n", com.c_str());
		pserial = this;
		puart = &huart1;
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
	DMA_HandleTypeDef * hdma = puart->hdmarx;

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
	SET_BIT(puart->Instance->CR3, USART_CR3_DMAR);	// enable DMA Receive

	HAL_NVIC_SetPriority(USART1_IRQn, 1 , 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

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
	va_list args;
	va_start(args, format);

	static uint8_t busy = 0;
	uint16_t length;

	while ( busy == 1 && READ_BIT( puart->Instance->SR, USART_SR_TC) == 0  )
	{}
	busy = 1;

	length = vsnprintf( tbuf,  tbufSize, (char*)format, args);
	va_end(args);
	emHAL_UART_Transmit_DMA(puart, (uint8_t*)( tbuf), length);
}

void Serial::readyRead(char * ch) {}
void Serial::emit_readyRead(char * data)
{
	emitWithParam(readyRead,(data),(data));
}	

void Serial::isr_handle(int ndtr)
{
	rCnt = rbufSize - ndtr;
	rbuf[rCnt] = '\0';
	//emit signal

	emit_readyRead(rbuf);
	
	//reset dma
	DMA_HandleTypeDef * hdma = puart->hdmarx;

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