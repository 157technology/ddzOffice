/*
 * Copyright (c) 2013-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Driver_Flash.h"
#include "w25q64.h"



#define ARM_FLASH_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0) /* driver version */

/* Sector Information */
#ifdef FLASH_SECTORS
static ARM_FLASH_SECTOR FLASH_SECTOR_INFO[FLASH_SECTOR_COUNT] = {
    FLASH_SECTORS
};
#else
#define FLASH_SECTOR_INFO    NULL
#endif

/* Flash Information */
static ARM_FLASH_INFO FlashInfo = {
    FLASH_SECTOR_INFO, 		/* FLASH_SECTOR_INFO  */
    256,	/* FLASH_SECTOR_COUNT */
    0x1000, /* FLASH_SECTOR_SIZE  */
    0x100, 	/* FLASH_PAGE_SIZE 256bytes    */
    0x1, 	/* FLASH_PROGRAM_UNIT */
    0xFF 	/* FLASH_ERASED_VALUE */
};

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus;
static uint8_t Flags;

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    0, /* event_ready */
    0, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    1  /* erase_chip */
};

//
// Functions
//
uint8_t W25Q64_WriteEnable(void);
SPI_HandleTypeDef * flash_spi = &hspi2;

ARM_DRIVER_VERSION ARM_Flash_GetVersion(void)
{
    return DriverVersion;
}

ARM_FLASH_CAPABILITIES ARM_Flash_GetCapabilities(void)
{
    return DriverCapabilities;
}


int32_t ARM_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
    uint8_t cmd[4] = {ENABLE_RESET, RESET};
    uint8_t s;

    (void)cb_event;

    FlashStatus.busy  = 0U;
    FlashStatus.error = 0U;

    W25Q64_Enable();
    HAL_SPI_Transmit(flash_spi, cmd, 2, SPI_TIMEOUT);
    W25Q64_Disable();

    cmd[0] = STATUS_REG1;
    W25Q64_Enable();
    HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
    HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
    W25Q64_Disable();

    uint8_t id[2];
    cmd[0] = DEV_ID;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = 0;
    W25Q64_Enable();
    HAL_SPI_Transmit(flash_spi, cmd, 4, SPI_TIMEOUT);
    HAL_SPI_Receive(flash_spi, id, 2, SPI_TIMEOUT);
    W25Q64_Disable();
    em_printf(">>>id is  %x %x\n", id[0], id[1]);


    if ( id[0] == 0xEF && id[1] == 0x16 )
        return ARM_DRIVER_OK;
    else
        return ARM_DRIVER_ERROR;
}

int32_t ARM_Flash_Uninitialize(void)
{
    HAL_SPI_MspDeInit(flash_spi);
    return 1;
}

int32_t ARM_Flash_PowerControl(ARM_POWER_STATE state)
{
    switch (state)
    {
    case ARM_POWER_OFF:
		//printf("Power off");
		FlashStatus.busy  = 0U;
		FlashStatus.error = 0U;
        break;

    case ARM_POWER_LOW:
		FlashStatus.busy  = 0U;
		FlashStatus.error = 0U;
        break;

    case ARM_POWER_FULL:
		FlashStatus.busy  = 0U;
		FlashStatus.error = 0U;
        break;

    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
	return ARM_DRIVER_OK;
}

int32_t ARM_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
    uint8_t cmd[4] = {READ_DATA};
    cmd[1] = (uint8_t)(addr >> 16);
    cmd[2] = (uint8_t)(addr >> 8);
    cmd[3] = (uint8_t)(addr);
	em_printf("FLASH>>>READ: 0x%X, %d.\n", addr, cnt);
    if ( data == NULL )
        return ARM_DRIVER_ERROR_PARAMETER;

    W25Q64_Enable();
    HAL_SPI_Transmit(flash_spi, cmd, 4, SPI_TIMEOUT);
    HAL_SPI_Receive(flash_spi, data, cnt, SPI_TIMEOUT);
    W25Q64_Disable();

    cmd[0] = STATUS_REG1;
    uint8_t s;
    W25Q64_Enable();
    HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
    HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
    W25Q64_Disable();
    if ( s & 0x01 )
        return ARM_DRIVER_ERROR_BUSY;
    else
        return (int32_t)cnt;
}
uint8_t W25Q64_WriteEnable(void)
{
    uint8_t cmd[] = {WRITE_ENABLE_CMD};
    uint32_t tickstart = HAL_GetTick();

    /*Select the FLASH: Chip Select low */
    W25Q64_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&hspi2, cmd, 1, SPI_TIMEOUT);
    /*Deselect the FLASH: Chip Select high */
    W25Q64_Disable();


    cmd[0] = STATUS_REG1;
    uint8_t s;
    W25Q64_Enable();
    HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
    HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
    W25Q64_Disable();
    while ( s & 0x01 )
    {
        if ((HAL_GetTick() - tickstart) > SPI_TIMEOUT)
        {
            return ARM_DRIVER_ERROR_TIMEOUT;
        }

        HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
        HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
        W25Q64_Disable();
    }
    return ARM_DRIVER_OK;
}
int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data, uint32_t cnt)
{
    if (((addr & 1U) != 0U) || (data == NULL))
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if (FlashStatus.busy)
    {
        return ARM_DRIVER_ERROR_BUSY;
    }

    if (cnt == 0U) { return 0; }
	em_printf("FLASH>>>WRITE: 0x%X, %d.\n", addr, cnt);
    FlashStatus.busy  = 1U;
    FlashStatus.error = 0U;

    uint8_t cmd[4];
    uint32_t end_addr, cur_size, cur_addr;
	uint8_t s;
    uint32_t tickstart = HAL_GetTick();

    /* Calculation of the size between the write address and the end of the page */
    cur_addr = 0;

    while (cur_addr <= addr)
    {
        cur_addr += FlashInfo.page_size;
    }
    cur_size = cur_addr - addr;

    /* Check if the size of the data is less than the remaining place in the page */
    if (cur_size > cnt)
    {
        cur_size = cnt;
    }

    /* Initialize the adress variables */
    cur_addr = addr;
    end_addr = addr + cnt;

    /* Perform the write page by page */
    do
    {
        /* Configure the command */
        cmd[0] = PAGE_PROG_CMD;
        cmd[1] = (uint8_t)(cur_addr >> 16);
        cmd[2] = (uint8_t)(cur_addr >> 8);
        cmd[3] = (uint8_t)(cur_addr);

        /* Enable write operations */
        W25Q64_WriteEnable();

        W25Q64_Enable();
        /* Send the command */
        if (HAL_SPI_Transmit(&hspi2, cmd, 4, SPI_TIMEOUT) != HAL_OK)
        {
            return ARM_DRIVER_ERROR_TIMEOUT;
        }

        /* Transmission of the data */
        if (HAL_SPI_Transmit(&hspi2, (uint8_t *)data, cur_size, SPI_TIMEOUT) != HAL_OK)
        {
            return ARM_DRIVER_ERROR_TIMEOUT;
        }
        W25Q64_Disable();
        /* Wait the end of Flash writing */

        cmd[0] = STATUS_REG1;
        W25Q64_Enable();
        HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
        HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
        W25Q64_Disable();
        while ( s & 0x01 )
        {
            /* Check for the Timeout */
            if ((HAL_GetTick() - tickstart) > SPI_TIMEOUT)
            {
                return ARM_DRIVER_ERROR_TIMEOUT;
            }
            W25Q64_Enable();
            HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
            HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
            W25Q64_Disable();
        }

        /* Update the address and size variables for next page programming */
        cur_addr += cur_size;
        data += cur_size;
        cur_size = ((cur_addr + FlashInfo.page_size) > end_addr) ? (end_addr - cur_addr) : FlashInfo.page_size;
    } while (cur_addr < end_addr);


    return ARM_DRIVER_OK;
}

int32_t ARM_Flash_EraseSector(uint32_t addr)
{
    FlashStatus.busy  = 1U;
    FlashStatus.error = 0U;

    uint8_t cmd[4];
    uint8_t s;
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = SECTOR_ERASE_CMD;
    cmd[1] = (uint8_t)(addr >> 16);
    cmd[2] = (uint8_t)(addr >> 8);
    cmd[3] = (uint8_t)(addr);

    /* Enable write operations */
    W25Q64_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Q64_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&hspi2, cmd, 4, SPI_TIMEOUT);
    /*Deselect the FLASH: Chip Select high */
    W25Q64_Disable();

    cmd[0] = STATUS_REG1;
    W25Q64_Enable();
    HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
    HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
    W25Q64_Disable();
    /* Wait the end of Flash writing */
    while ( s & 0x01 )
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > ERASE_TIME_OUT)
        {
            return ARM_DRIVER_ERROR_TIMEOUT;
        }
        W25Q64_Enable();
        HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
        HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
        W25Q64_Disable();
    }
	FlashStatus.busy  = 0U;
    FlashStatus.error = 0U;
    return ARM_DRIVER_OK;
}

int32_t ARM_Flash_EraseChip(void)
{
    uint8_t cmd[4];
    uint8_t s;
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = SECTOR_ERASE_CMD;

    /* Enable write operations */
    W25Q64_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Q64_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&hspi2, cmd, 1, SPI_TIMEOUT);
    /*Deselect the FLASH: Chip Select high */
    W25Q64_Disable();
    cmd[0] = STATUS_REG1;
    W25Q64_Enable();
    HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
    HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
    W25Q64_Disable();

    /* Wait the end of Flash writing */
    while ( s & 0x01 )
    {
        if ((HAL_GetTick() - tickstart) > ERASE_ALL_TIME_OUT)
        {
            return ARM_DRIVER_ERROR_TIMEOUT;
        }
        W25Q64_Enable();
        HAL_SPI_Transmit(flash_spi, cmd, 1, SPI_TIMEOUT);
        HAL_SPI_Receive(flash_spi, &s, 1, SPI_TIMEOUT);
        W25Q64_Disable();
    }
	FlashStatus.busy  = 0U;
	FlashStatus.error = 0U;
    return ARM_DRIVER_OK;
}

ARM_FLASH_STATUS ARM_Flash_GetStatus(void)
{
    return FlashStatus;
}

ARM_FLASH_INFO * ARM_Flash_GetInfo(void)
{
    return &FlashInfo;
}

void ARM_Flash_SignalEvent(uint32_t event)
{
}
// End Flash Interface
extern
ARM_DRIVER_FLASH ARM_Driver_Flash_(0);
ARM_DRIVER_FLASH ARM_Driver_Flash_(0) = {
    ARM_Flash_GetVersion,
    ARM_Flash_GetCapabilities,
    ARM_Flash_Initialize,
    ARM_Flash_Uninitialize,
    ARM_Flash_PowerControl,
    ARM_Flash_ReadData,
    ARM_Flash_ProgramData,
    ARM_Flash_EraseSector,
    ARM_Flash_EraseChip,
    ARM_Flash_GetStatus,
    ARM_Flash_GetInfo
};
