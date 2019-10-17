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
#include "w25qxx.h"
#include "serial.h"
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
    0, /* FLASH_SECTOR_INFO  */
    2048,//2048, /* FLASH_SECTOR_COUNT */
    0x1000,//0x1000, /* FLASH_SECTOR_SIZE  */
    0x100, /* FLASH_PAGE_SIZE    */
    1, /* FLASH_PROGRAM_UNIT */
    0xFF  /* FLASH_ERASED_VALUE */
};

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus;

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    0, /* event_ready */
    0, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    0  /* erase_chip */
};

//
// Functions
//

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
	FlashStatus.busy  = 0U;
	FlashStatus.error = 0U;
	if ( W25qxx_Init() )	return ARM_DRIVER_OK;
	else					return ARM_DRIVER_ERROR;
}

int32_t ARM_Flash_Uninitialize(void)
{
	return ARM_DRIVER_OK;
}

int32_t ARM_Flash_PowerControl(ARM_POWER_STATE state)
{
    switch (state)
    {
    case ARM_POWER_OFF:
		return ARM_DRIVER_OK;
        break;

    case ARM_POWER_LOW:
		return ARM_DRIVER_OK;
        break;

    case ARM_POWER_FULL:
		return ARM_DRIVER_OK;
        break;

    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
	return ARM_DRIVER_OK;
}

int32_t ARM_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
	W25qxx_ReadBytes(data, addr, cnt);
	return ARM_DRIVER_OK;
}

int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data, uint32_t cnt)
{
	uint8_t * buf = (uint8_t *)data;
	for ( int i = 0; i < cnt; i ++ )
	{
		W25qxx_WriteByte(buf[i], addr+i);
	}
	
	return ARM_DRIVER_OK;
}

uint32_t cnt = 0;
int32_t ARM_Flash_EraseSector(uint32_t addr)
{
	//em_printf("CNT::%5u\n", cnt ++);
	//HAL_Delay(1);
	cnt ++;
	W25qxx_EraseSector(addr);
	return ARM_DRIVER_OK;
}

int32_t ARM_Flash_EraseChip(void)
{
	W25qxx_EraseChip();
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