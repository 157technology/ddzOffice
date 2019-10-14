#ifndef __W35Q64_H__
#define __W35Q64_H__


#include "Driver_Flash.h"
#include "spi.h"
#include "gpio.h"
#include "main.h"
#include "serial.h"



#define W25Q64_Enable()   HAL_GPIO_WritePin(Flash_CS_GPIO_Port, Flash_CS_Pin, GPIO_PIN_RESET)
#define W25Q64_Disable()  HAL_GPIO_WritePin(Flash_CS_GPIO_Port, Flash_CS_Pin, GPIO_PIN_SET)

#define SPI_TIMEOUT	 1000



#define PAGE_PROG_CMD           0x02
#define READ_DATA	 			0x03
#define WRITE_DISABLE_CMD       0x04
#define STATUS_REG1 			0x05
#define WRITE_ENABLE_CMD        0x06
#define SECTOR_ERASE_CMD        0x20
#define ENABLE_RESET			0x66
#define DEV_ID		 			0x90
#define RESET 		 			0x99
#define CHIP_ERASE_CMD          0xC7


#define ERASE_TIME_OUT          3000
#define ERASE_ALL_TIME_OUT      250000

#define RT_TIMEOUT				10
#define RT_OK					1




//extern ARM_DRIVER_FLASH Driver_FLASH0;

int32_t ARM_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event);
int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data, uint32_t cnt);
int32_t ARM_Flash_EraseSector(uint32_t addr);
int32_t ARM_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt);

#endif