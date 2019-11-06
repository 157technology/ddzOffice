#ifndef __RTX_MAIN_H
#define __RTX_MAIN_H 

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"


#include "serial.h"
#include "w25qxx.h"
#include "OLED_I2C.h"
#include <string.h>
#include <math.h>

//#include "shell.h"

void rtx_main(void);


///
///
///
int Init_GUIThread (void);

#endif