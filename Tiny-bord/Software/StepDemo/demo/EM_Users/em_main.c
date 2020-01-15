/*
 * @Author: your name
 * @Date: 2020-01-14 10:42:46
 * @LastEditTime : 2020-01-15 14:43:27
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

void SerialSlot(void * str)
{
    em_printf("Slot: %s\r\n", (char*)str);
    Serial_Print(pwifi->link, "%s\r\n", (char*)str);
}

void mainThread(void * argument);

int main(void)
{
    connect(testSignal,testSlot, DirectConnect);
    connect(console->signal, SerialSlot, DirectConnect);
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
    //char ch[50];
    int cnt = 0;
    creat_signalThread();

    WifiInit();

    while (1)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        osDelay(200);
        emit(testSignal, NULL);

        if ( cnt ++ == 50 )
        {
            cnt = 0;
			//ch[0] = '\0';
            //pwifi->pqueue->readAll(ch, 2000);
            //em_printf(">>> All: %s", ch);
            // if ( WIFI_AT_CMD() == wfOk )
            // {
            //     em_printf("AT test OK.\r\n");
            // }
        }
    }
    
}