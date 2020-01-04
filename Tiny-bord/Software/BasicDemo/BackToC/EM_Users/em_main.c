#include "commom.h"

Serial *console;
Wifi *wifi;

void sigKey(void *num) {}
void sigBtn(void *num) {}
void slotkey(void *num)
{
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}
void slotBtn(void *num)
{
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}
void slotSerial(void *data)
{
    char *str = (char *)data;
    em_printf("Console Get: %s.\n", (char *)data);
    if (str[0] == '1')
    {
        //Serial_Print(wifi_serial, "AT\r\n");
    }
    // else if (str[0] == '2')
    // {
    //     Serial_Print(wifi_serial, "AT+RST\r\n");
    // }
    // else if (str[0] == '3')
    // {
    //     Serial_Print(wifi_serial, "AT+GMR\r\n");
    // }
    // else if (str[0] == '4')
    // {
    //     Serial_Print(wifi_serial, "AT+CWJAP_DEF=\"emeili\",\"88888888\"\r\n");
    // }
    // else if (str[0] == 'q')
    // {
    //     Serial_Print(wifi_serial, "+++");
    //     /* code */
    // }
    // else if (str[0] == 'u')
    // {
    //     Serial_Print(wifi_serial, "AT+CIUPDATE\r\n");
    //     /* updata */
    // }
    // else if (str[0] == 't')
    // {
    //     Serial_Print(wifi_serial, "+++");
    //     /* tcp */
    // }
    // else if (str[0] == 's')
    // {
    //     Serial_Print(wifi_serial, "AT+CWLAP\r\n");
    //     /* show aps */
    // }
    // else if (str[0] == 'i')
    // {
    //     Serial_Print(wifi_serial, "AT+CIPSTATUS\r\n");
    //     /* show aps */
    // }
    //     else if (str[0] == 'o')
    // {
    //     Serial_Print(wifi_serial, "AT+CIFSR\r\n");
    //     /* show aps */
    // }
    //         else if (str[0] == 'p')
    // {
    //     Serial_Print(wifi_serial, "AT+PING=\"www.baidu.com\"\r\n");
    //     /* show aps */
    // }
}

void main_thread(void *argument)
{
    //GUI_DispStringAt("Hello World.......", 0, 18);
    creat_signalThread();
    socket tcp;
    //WifiSelfCheck();
    if ( WifiInit() == wfOk )
    {
        tcp = TcpSocket("192.168.137.1", 2222);
        
    }
    

    //WifiSmart();
        
    while (1)
    {
        /* code */
        //OLED_Replot();
        //HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
        osDelay(1000); // max 25 fps
        TcpSend(tcp, "Hello", 5);
        emit(sigBtn, NULL);
    }
}

/**/
static const osThreadAttr_t GUIThread_attr = {
    .priority = osPriorityLow};

int main(void)
{
    console = Serial_Regester(&huart1, 512, 512);
    //wifi_serial = Serial_Regester(&huart6, 512, 512);
    Serial_Init(console);
    wifi = Wifi_Regester(&huart6, 512, 512);
    //Serial_Init(wifi_serial);

    em_printf("Hello World\n");
    connect(ReadyRead, slotSerial);
    //connect(WifiRead, slotWifi);

    connect(sigBtn, slotkey);
    connect(sigKey, slotkey);
    connect(sigKey, slotBtn);
    connect(sigBtn, slotBtn);

    SystemCoreClockUpdate();
    osKernelInitialize();                            // Initialize CMSIS-RTOS
    osThreadNew(main_thread, NULL, &GUIThread_attr); // Create application main thread
    osKernelStart();                                 // Start thread execution
}
