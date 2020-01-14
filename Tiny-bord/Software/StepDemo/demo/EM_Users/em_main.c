/*
 * @Author: your name
 * @Date: 2020-01-14 10:42:46
 * @LastEditTime : 2020-01-14 16:53:18
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \demo\EM_Users\em_main.c
 */
#include "commom.h"

void testSignal(void*n){}
void testSlot(void * argument)
{
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}
void tN(void * argument)
{
    HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
}
void bN(void*argument)
{
    HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
}

void mainThread(void * argument);

int main(void)
{
    connect(testSignal,testSlot, DirectConnect);
    connect(testSignal, tN, QueuedConnect);
    connect(testSignal, bN, BlockingQueuedConnect);
	EventRecorderInitialize(EventRecordAll, 1);
    // start rtos
    SystemCoreClockUpdate();
    osKernelInitialize();
    osThreadNew(mainThread, NULL, NULL);
    osKernelStart();
    return 0;
}

void mainThread(void * argument)
{
    creat_signalThread();
    while (1)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        osDelay(200);
        emit(testSignal, NULL);
    }
    
}