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
#define CLOUD "a1eQDqcFTdO.iot-as-mqtt.cn-shanghai.aliyuncs.com"
char mqttbuf[200];
char teststr[] = "mqttbuf";
void main_thread(void *argument)
{
	int m_cnt = 0;
    static int flag = 0;
    //GUI_DispStringAt("Hello World.......", 0, 18);
    creat_signalThread();
    socket tcp;
    socket tcp1;
    socket mqtt;
    int32_t len;
    int buflen = sizeof(mqttbuf);
    MQTTString topicString = MQTTString_initializer;
    int req_qos = 1;
    unsigned short msgid = 1;
    //WifiSelfCheck();
    if (WifiInit() == wfOk)
    {
        // tcp = TcpSocket("192.168.137.1", 2222);
        // if (tcp != -1)
        //     flag = 1;

        // tcp1 = TcpSocket("192.168.137.1", 3333);
        // if (tcp1 != -1)
        //     flag = 1;

        mqtt = transport_open(CLOUD, 1883);
        //mqtt = transport_open("192.168.137.1", 3333);
        if (mqtt != -1)
        {
            wifi->sock_mqtt = mqtt;

            //		int msgid = 1;
            //		int req_qos = 0;

            MQTTPacket_connectData data = MQTTPacket_connectData_initializer; //ÅäÖÃ²¿·Ö¿É±äÍ·²¿µÄÖµ

            //productkey    a1eQDqcFTdO
            //device name   0gqfJL6z7j8HvQQbFQQY
            //device s      BbQA78oIs6DAWQYGfpI22rUY9iqwIF7d
            //    MQTTString topicString = MQTTString_initializer;

            memset(mqttbuf, 0, buflen);
            data.clientID.cstring = "test|securemode=3,signmethod=hmacsha1|";   //¿Í»§¶Ë±êÊ¶£¬ÓÃÓÚÇø·ÖÃ¿¸ö¿Í»§¶ËxxxÎª×Ô¶¨Òå£¬ºóÃæÎª¹Ì¶¨¸ñÊ½
            data.keepAliveInterval = 300;                                       //±£»î¼ÆÊ±Æ÷£¬¶¨ÒåÁË·þÎñÆ÷ÊÕµ½¿Í»§¶ËÏûÏ¢µÄ×î´óÊ±¼ä¼ä¸ô
            data.cleansession = 1;                                              //¸Ã±êÖ¾ÖÃ1·þÎñÆ÷±ØÐë¶ªÆúÖ®Ç°±£³ÖµÄ¿Í»§¶ËµÄÐÅÏ¢£¬½«¸ÃÁ¬½ÓÊÓÎª¡°²»´æÔÚ¡±
            data.username.cstring = "0gqfJL6z7j8HvQQbFQQY&a1eQDqcFTdO";         //ÓÃ»§Ãû DeviceName&ProductKey
            data.password.cstring = "3C14027F650D896ECEF8C5606BEC47F199A8B38C"; //ÃÜÂë£¬¹¤¾ßÉú³É
            len = MQTTSerialize_connect(mqttbuf, buflen, &data);                /*1 ¹¹ÔìÁ¬½Ó±¨ÎÄ*/
            mqttbuf[len] = '\0';
            transport_sendPacketBuffer(mqtt, mqttbuf, len); //·¢ËÍÁ¬½ÓÇëÇó
            //transport_sendPacketBuffer(mqtt, teststr, 7);
            {
                unsigned char sessionPresent, connack_rc;
                do
                {
                    while (MQTTPacket_read(mqttbuf, buflen, transport_getdata) != CONNACK) //¶Ô½ÓÊÕµ½µÄ±¨ÎÄ½øÐÐ½âÎö
                    {
                        //osDelay(100);
                    }
                } while (MQTTDeserialize_connack(&sessionPresent, &connack_rc, mqttbuf, buflen) != 1 || connack_rc != 0);
            }
            wifi->mqtt_read = 0;
            //mqtt初始化成功
            osDelay(500);
            topicString.cstring = "/a1eQDqcFTdO/0gqfJL6z7j8HvQQbFQQY/user/test";
            len = MQTTSerialize_subscribe(mqttbuf, buflen, 0, msgid, 1, &topicString, &req_qos);
            transport_sendPacketBuffer(mqtt, mqttbuf, len);
            em_printf("Wait ACK...\n");
            while (MQTTPacket_read(mqttbuf, buflen, transport_getdata) != SUBACK) /* wait for suback */
            {
                ;
            }
            em_printf("ACK OK\n");
            {
                unsigned short submsgid;
                int subcount;
                int granted_qos;
                MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, mqttbuf, buflen);
                if (granted_qos != 0)
                {
                    em_printf("granted qos != 0, %d\n", granted_qos);
                    //goto exit;
                }
            }
        }
    }

    //WifiSmart();
    wifi->mqtt_read = 0;
    int timecount = 0;
    int time_1s = 0;
    while (1)
    {
        /* code */
        //OLED_Replot();
        //HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
        osDelay(5); // max 25 fps

        if (timecount++ == 16000)
        {
            len = MQTTSerialize_pingreq(mqttbuf, buflen);
            transport_sendPacketBuffer(mqtt, mqttbuf, len);
            timecount = 0;
        }

        if (MQTTPacket_read(mqttbuf, buflen, transport_getdata_once) == PUBLISH)
        {
            unsigned char dup;
            int qos;
            unsigned char retained;
            unsigned short msgid;
            int payloadlen_in;
            unsigned char *payload_in;
            int rc;
            MQTTString receivedTopic;
            MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
                                    &payload_in, &payloadlen_in, mqttbuf, buflen);

            //em_printf("receive topic: %s %d\n", receivedTopic.lenstring.data, receivedTopic.lenstring.len);

            //em_printf("message arrived %.*s\n", payloadlen_in, payload_in);

            len = MQTTSerialize_puback(mqttbuf, buflen, msgid);
            transport_sendPacketBuffer(mqtt, mqttbuf, len);
            wifi->mqtt_read = 0;
        }

        //1s
        if (time_1s++ == 200)
        {
			time_1s = 0;
            cJSON *pJsonRoot = NULL;
            pJsonRoot = cJSON_CreateObject();
            if (pJsonRoot == NULL)
            {
                em_printf("Json Failed.\n");
                continue;
            }
            cJSON_AddStringToObject(pJsonRoot, "id", "rty");                           //
            cJSON_AddStringToObject(pJsonRoot, "method", "thing.event.property.post"); //
            cJSON *pJsonChild = cJSON_CreateObject();                                  //
            if (NULL == pJsonChild)
            {
                cJSON_Delete(pJsonChild);
                continue;
            }
            cJSON_AddNumberToObject(pJsonChild, "CurrentTemperature", m_cnt++);
            cJSON_AddItemToObject(pJsonRoot, "params", pJsonChild); //

            char *lpJsonStr = cJSON_Print(pJsonRoot);
            cJSON_Delete(pJsonRoot); //
            if (lpJsonStr == NULL)
            {
                em_printf("lpJsonStr Failed.\n");
                continue;
            }

            topicString.cstring = "/sys/a1eQDqcFTdO/0gqfJL6z7j8HvQQbFQQY/thing/event/property/post";
            len = MQTTSerialize_publish(mqttbuf, buflen, 0, 2, 0, 0, topicString, (unsigned char*)lpJsonStr, strlen(lpJsonStr));
            transport_sendPacketBuffer(mqtt, mqttbuf, len);
            free(lpJsonStr);
        }

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
