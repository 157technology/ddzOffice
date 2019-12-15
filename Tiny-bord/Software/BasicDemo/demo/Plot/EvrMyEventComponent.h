/*------------------------------------------------------------------------------
 * MyEventComponent - Component ::MDK Example:MyEventComponent
 * Copyright (c) 2018 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    EvrMyEventComponent.h
 * Purpose: Definition of debug events for MyEventComponent example
 * Rev.:    V1.0.0
 *----------------------------------------------------------------------------*/
  

#include "EventRecorder.h"              // Keil::Compiler:Event Recorder


#ifndef __STATIC_INLINE
  #define __STATIC_INLINE           static __inline
#endif
  

// Notation used in the MyEventComponent example:
//
// The following prefix is used for identifiers:
#define MyEvtC1 0x0B            /// < Number of the component with short name 'MyEvtC1'
#define MyEvtC2 0x0C            /// < Number of the component with short name 'MyEvtC2'

#ifdef MyEvtC_DEBUG             // Define debug events
  
// Event id list for "MyEventComponent"
#define EvtMyC1_Clear             EventID (EventLevelAPI,   MyEvtC1, 0x00)
#define EvtMyC1_Create            EventID (EventLevelAPI,   MyEvtC1, 0x01)
#define EvtMyC1_Delete            EventID (EventLevelAPI,   MyEvtC1, 0x02)
#define EvtMyC1_Active            EventID (EventLevelAPI,   MyEvtC1, 0x03)
#define EvtMyC1_Waiting           EventID (EventLevelAPI,   MyEvtC1, 0x04)
#define EvtMyC1_Idle              EventID (EventLevelAPI,   MyEvtC1, 0x05)
#define EvtMyC1_Stalled           EventID (EventLevelAPI,   MyEvtC1, 0x06)

#define EvtMyC2_Clear             EventID (EventLevelAPI,   MyEvtC2, 0x00)
#define EvtMyC2_Create            EventID (EventLevelAPI,   MyEvtC2, 0x01)
#define EvtMyC2_Delete            EventID (EventLevelAPI,   MyEvtC2, 0x02)
#define EvtMyC2_Active            EventID (EventLevelAPI,   MyEvtC2, 0x03)
#define EvtMyC2_Waiting           EventID (EventLevelAPI,   MyEvtC2, 0x04)
#define EvtMyC2_Idle              EventID (EventLevelAPI,   MyEvtC2, 0x05)
#define EvtMyC2_Stalled           EventID (EventLevelAPI,   MyEvtC2, 0x06)

#define EvtMyC2_Reset             EventID (EventLevelAPI,   MyEvtC2, 0xF0)

#endif // MyEvtC_DEBUG


/**
  \brief  Event on \ref MyC1_Clear
 */
#ifdef EvtMyC1_Clear
  __STATIC_INLINE void EvrMyC1_Clear(unsigned int v) {
    EventRecord2(EvtMyC1_Clear, v, 0);
  }
#else
  #define EvrMyC1_Clear(v)
#endif

/**
  \brief  Event on \ref MyC1_Create
 */
#ifdef EvtMyC1_Create
  __STATIC_INLINE void EvrMyC1_Create(unsigned int v, unsigned int h) {
    EventRecord2(EvtMyC1_Create, v, h);
  }
#else
  #define EvrMyC1_Create(v)
#endif

/**
  \brief  Event on \ref MyC1_Delete
 */
#ifdef EvtMyC1_Delete
  __STATIC_INLINE void EvrMyC1_Delete(unsigned int v) {
    EventRecord2(EvtMyC1_Delete, v, 0);
  }
#else
  #define EvrMyC1_Delete(v)
#endif
  
/**
  \brief  Event on \ref MyC1_Active
 */
#ifdef EvtMyC1_Active
  __STATIC_INLINE void EvrMyC1_Active(unsigned int v) {
    EventRecord2(EvtMyC1_Active, v, 0);
  }
#else
  #define EvrMyC1_Active(v)
#endif
  
/**
  \brief  Event on \ref MyC1_Waiting_Blocking
 */
#ifdef EvtMyC1_Waiting
  __STATIC_INLINE void EvrMyC1_Waiting(unsigned int v) {
    EventRecord2(EvtMyC1_Waiting, v, 0);
  }
#else
  #define EvrMyC1_Waiting(v)
#endif

/**
  \brief  Event on \ref MyC1_Idle
 */
#ifdef EvtMyC1_Idle
  __STATIC_INLINE void EvrMyC1_Idle(unsigned int v) {
    EventRecord2(EvtMyC1_Idle, v, 0);
  }
#else
  #define EvrMyC1_Idle(v)
#endif
  
/**
  \brief  Event on \ref MyC1_Stalled
 */
#ifdef EvtMyC1_Stalled
  __STATIC_INLINE void EvrMyC1_Stalled(unsigned int v) {
    EventRecord2(EvtMyC1_Stalled, v, 0);
  }
#else
  #define EvrMyC1_Stalled(v)
#endif


  
/**
  \brief  Event on \ref MyC2_Clear
 */
#ifdef EvtMyC2_Clear
  __STATIC_INLINE void EvrMyC2_Clear(unsigned int v) {
    EventRecord2(EvtMyC2_Clear, v, 0);
  }
#else
  #define EvrMyC2_Clear()
#endif

/**
  \brief  Event on \ref MyC2_Create
 */
#ifdef EvtMyC2_Create
  __STATIC_INLINE void EvrMyC2_Create(unsigned int v) {
    EventRecord2(EvtMyC2_Create, v, 0);
  }
#else
  #define EvrMyC2_Create()
#endif

/**
  \brief  Event on \ref MyC2_Delete
 */
#ifdef EvtMyC2_Delete
  __STATIC_INLINE void EvrMyC2_Delete(unsigned int v) {
    EventRecord2(EvtMyC2_Delete, v, 0);
  }
#else
  #define EvrMyC2_Delete()
#endif
  
/**
  \brief  Event on \ref MyC2_Active
 */
#ifdef EvtMyC2_Active
  __STATIC_INLINE void EvrMyC2_Active(unsigned int v) {
    EventRecord2(EvtMyC2_Active, v, 0);
  }
#else
  #define EvrMyC2_Active()
#endif
  
/**
  \brief  Event on \ref MyC2_Waiting
 */
#ifdef EvtMyC2_Waiting
  __STATIC_INLINE void EvrMyC2_Waiting(unsigned int v) {
    EventRecord2(EvtMyC2_Waiting, v, 0);
  }
#else
  #define EvrMyC2_Waiting()
#endif

/**
  \brief  Event on \ref MyC2_Idle
 */
#ifdef EvtMyC2_Idle
  __STATIC_INLINE void EvrMyC2_Idle(unsigned int v) {
    EventRecord2(EvtMyC2_Idle, v, 0);
  }
#else
  #define EvrMyC2_Idle()
#endif
  
/**
  \brief  Event on \ref MyC2_Stalled
 */
#ifdef EvtMyC2_Stalled
  __STATIC_INLINE void EvrMyC2_Stalled(unsigned int v) {
    EventRecord2(EvtMyC2_Stalled, v, 0);
  }
#else
  #define EvrMyC2_Stalled()
#endif
  
/**
  \brief  Event on \ref MyC2_Reset
 */
#ifdef EvtMyC2_Reset
  __STATIC_INLINE void EvrMyC2_Reset(void) {
    EventRecord2(EvtMyC2_Reset, 0, 0);
  }
#else
  #define EvrMyC2_Reset()
#endif
