#include "cmsis_os2.h"                          // CMSIS RTOS header file
//#include "signalslot.hpp"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/



osThreadId_t tid_SignalSlot_Thread;                        // thread id
osSemaphoreId_t semSignalEmit;
void SignalSlot(void *argument);                   // thread function

int Init_SignalSlot_Thread (void)
{
    tid_SignalSlot_Thread = osThreadNew(SignalSlot, NULL, NULL);
    if (tid_SignalSlot_Thread == NULL)
    {
        return (-1);
    }
    return (0);
}

void SignalSlot (void *argument)
{
    semSignalEmit = osSemaphoreNew(10U, 0U, NULL);
    while (1)
    {
        osSemaphoreAcquire(semSignalEmit, osWaitForever);
//		signalslot_t f = m_stack.top();
//		f.slot(f.para);
//		m_stack.pop();
    }
}
