/*------------------------------------------------------------------------------
 * MyComponent - Component ::MDK Example:MyComponent
 * Copyright (c) 2016 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    MyComponent.h
 * Purpose: Example showing the usage of Software Component Views in MDK
 * Rev.:    V1.0.1
 *----------------------------------------------------------------------------*/
 #ifdef __cplusplus
extern "C" {
#endif


extern void event_state (void *argument);
extern void event_state_init (void);
#ifdef __cplusplus
}
#endif
// header file that defines debug events
#include "EvrMyEventComponent.h"
