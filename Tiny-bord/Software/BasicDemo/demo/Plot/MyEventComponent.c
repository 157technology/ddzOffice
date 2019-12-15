/*------------------------------------------------------------------------------
 * MyEventComponent - Component ::MDK Example:MyEventComponent
 * Copyright (c) 2018 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    MyEventComponent.c
 * Purpose: Example showing the usage of Status Views in MDK
 * Rev.:    V1.0.0
 *----------------------------------------------------------------------------*/
 
 
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MyEvtC_DEBUG                    // enable debug events

#include "MyEventComponent.h"
#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5

#include "main.h"


unsigned int h;
void event_state (void *argument) {
  unsigned int i, name;
  
  name = (unsigned int) argument;
  h = name + 0x10000000;
  
  HAL_Delay(100);
  EvrMyC1_Create (h, name);
  for (i=0; i < 2; i++)  {
    HAL_Delay(100);
    EvrMyC1_Active (h);
    HAL_Delay(100);
    EvrMyC1_Waiting (h);
    HAL_Delay(100);
    EvrMyC1_Idle (h);
    HAL_Delay(100);
  }
  
  //EvrMyC1_Clear (h);
  HAL_Delay(100);
  //EvrMyC1_Delete (h);

}


void event_state_init (void)  {
  EvrMyC2_Reset ();                     // Event on component initalization
}

