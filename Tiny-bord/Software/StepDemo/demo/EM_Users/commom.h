/*
 * @Author: your name
 * @Date: 2020-01-14 10:42:46
 * @LastEditTime : 2020-01-14 16:53:06
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \demo\EM_Users\commom.h
 */
#ifndef __COMMON_H__
#define __COMMON_H__

// 系统内部
#include "../Core/Inc/main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

// 标准库
#include <stdio.h>
// 组件
// #include "w25qxx.h"
// #include "oled.h"
//#include "sht21.h"
//#include "key.h"
#include "serial.h"
#include "memory.h"
#include "wifi.h"
// #include "transport.h"
// #include "MQTTPacket.h"
// #include "MQTTConnect.h"

#include "signal_slot.h"

// OS
#include "../MDK-ARM/RTE/_demo/EventRecorder.h"
#include "../MDK-ARM/RTE/_demo/RTE_Components.h" 
#include  CMSIS_device_header
#include "cmsis_os2.h"

// GUI
//#include "GUI.h"                        // Segger.MDK-Plus::Graphics:CORE

// file sys
//#include "rl_fs.h"                      // Keil.MDK-Plus::File System:CORE

// #include "cJSON.h"                      // MDK-Packs::Data Exchange:JSON:cJSON

/*=======================*/
#ifdef __cplusplus
extern "C"
{
#endif
    extern void SystemClock_Config(void);
#ifdef __cplusplus
}
#endif
/*=======================*/

#endif