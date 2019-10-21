#ifndef __OSMAIN_H__
#define __OSMAIN_H__ 

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"


#include <serial.h>
#include "w25qxx.h"
#include <string.h>
#include <math.h>

#include "shell.h"

int osMain(void);

#endif