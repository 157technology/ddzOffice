#ifndef __COMMON_HPP__
#define __COMMON_HPP__ 

#include "main.h"
#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "tim.h"
//############################
//
#include "oled.h"
#include "memory.h"
#include "serial.h"

extern int main();

/*##################################*/
// OS
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

extern void osSetup(void);
/*##################################*/




/*##################################*/
#ifdef __cplusplus
extern "C" {
#endif
	
extern void SystemClock_Config(void);
	extern int Init_GUIThread (void);
	
#ifdef __cplusplus
}
#endif
/*##################################*/
	

#endif