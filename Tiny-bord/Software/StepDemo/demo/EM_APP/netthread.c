/*
 * @Author: your name
 * @Date: 2020-01-16 09:42:24
 * @LastEditTime : 2020-01-17 08:59:42
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \demo\EM_APP\netthread.c
 */
#include "netthread.h"
#include "cmsis_os2.h"
#include "wifi.h"
#include "transport.h"
#include "MQTTPacket.h"
#include "MQTTConnect.h"

#include "rtc.h"

/*初始化mqtt*/
#define ALICLOUD "a1eQDqcFTdO.iot-as-mqtt.cn-shanghai.aliyuncs.com"
int mqtt_init();
int mqtt_publish();
int mqtt_subscription();
int mqtt_receive();
void mqtt_heartbeat();

void http_updatetime();

char databuf[] = "123456789abcdefghijklmn\r\n";
void NetThread(void *argument)
{
    while (WifiInit() != wfOk)
    {
        em_printf("Wifi Init ERROR.\r\nTry Later.\r\n");
        osDelay(1000);
    }
    http_updatetime();
    osDelay(3000);
    if (mqtt_init() != 1)
    {
        em_printf("MQTT INIT FAILED.\r\n");
    }

    //socket tcp1 = TcpSocket("192.168.137.1", 3333);
    while (1)
    {
        // if (TcpSend(tcp1, databuf, strlen(databuf)) != wfOk)
        //     em_printf(">Send ERROR.\r\n");

        //mqtt_receive();
        osDelay(120000);
        mqtt_heartbeat();
    }
}

void creat_NetThread()
{
    osThreadAttr_t signalThread_attr = {
        .priority = osPriorityNormal1};
    osThreadNew(NetThread, NULL, &signalThread_attr);
}

int mqtt_init()
{
    char buf[512];
    int buflen = 512;

    socket mqtt = transport_open(ALICLOUD, 1883);
    if (mqtt == -1)
    {
        return -1;
    }
    //em_printf(">mqtt init\r\n");
    pwifi->sock_mqtt = mqtt;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    memset(buf, 0, buflen);

    data.clientID.cstring = "test|securemode=3,signmethod=hmacsha1|";
    data.keepAliveInterval = 300;
    data.cleansession = 1;
    data.username.cstring = "0gqfJL6z7j8HvQQbFQQY&a1eQDqcFTdO";
    data.password.cstring = "3C14027F650D896ECEF8C5606BEC47F199A8B38C";

    int len = MQTTSerialize_connect(buf, buflen, &data);
    transport_sendPacketBuffer(mqtt, buf, len);

    {
        unsigned char sessionPresent, connack_rc;
        do
        {
            while (MQTTPacket_read(buf, buflen, transport_getdata) != CONNACK) //
            {
            }
        } while (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0);
    }

    return 1;
}

void mqtt_heartbeat()
{
    char buf[32];
    int buflen = 32;
    int len = MQTTSerialize_pingreq(buf, buflen);
    transport_sendPacketBuffer(pwifi->sock_mqtt, buf, len);
}

int mqtt_receive()
{
    char buf[512];
    int buflen = 512;
    pwifi->mqttqueue->clear();
    int type = MQTTPacket_read(buf, buflen, transport_getdata);
    //em_printf("\r\n>mqtt receive. %d\r\n", type);
    if (type == PUBLISH)
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
                                &payload_in, &payloadlen_in, buf, buflen);

        int len = MQTTSerialize_puback(buf, buflen, msgid);
        transport_sendPacketBuffer(pwifi->sock_mqtt, buf, len);

        em_printf(">Receive Topic: %.*s.\r\n", receivedTopic.lenstring.len, receivedTopic.lenstring.data);
        em_printf(">Receive Data : %.*s.\r\n", payloadlen_in, payload_in);
        //em_printf(">mqtt receive. %d\r\n", ttt);
        return 1;
    }
    //em_printf(">%d\r\n", ttt);
    return -1;
}
int mqtt_subscription(char *topic)
{
    char buf[512];
    int buflen = 512;
    int req_qos = 1;
    unsigned short msgid = 1;
    MQTTString topicString = MQTTString_initializer;
    sprintf(topicString.cstring, "/a1eQDqcFTdO/0gqfJL6z7j8HvQQbFQQY/user/%s", topic);

    int len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);
    transport_sendPacketBuffer(pwifi->sock_mqtt, buf, len);
    while (MQTTPacket_read(buf, buflen, transport_getdata) != SUBACK) /* wait for suback */
    {
        ;
    }
    return 1;
}

int m_str2weekday(char *str)
{
    if (strncmp(str, "Mon", 3) == 0)
    {
        return 1;
    }
    if (strncmp(str, "Tus", 3) == 0)
    {
        return 2;
    }
    if (strncmp(str, "Wen", 3) == 0)
    {
        return 3;
    }
    if (strncmp(str, "Thu", 3) == 0)
    {
        return 4;
    }
    if (strncmp(str, "Fri", 3) == 0)
    {
        return 5;
    }
    if (strncmp(str, "Sat", 3) == 0)
    {
        return 6;
    }
    if (strncmp(str, "Sun", 3) == 0)
    {
        return 7;
    }
    return 0;
}
int m_str2month(char *str)
{
    if (strncmp(str, "Jan", 3) == 0)
    {
        return 1;
    }
    if (strncmp(str, "Feb", 3) == 0)
    {
        return 2;
    }
    if (strncmp(str, "Mar", 3) == 0)
    {
        return 3;
    }
    if (strncmp(str, "Apr", 3) == 0)
    {
        return 4;
    }
    if (strncmp(str, "May", 3) == 0)
    {
        return 5;
    }
    if (strncmp(str, "Jun", 3) == 0)
    {
        return 6;
    }
    if (strncmp(str, "Jul", 3) == 0)
    {
        return 7;
    }
    if (strncmp(str, "Aug", 3) == 0)
    {
        return 8;
    }
    if (strncmp(str, "Sep", 3) == 0)
    {
        return 9;
    }
    if (strncmp(str, "Oct", 3) == 0)
    {
        return 0x10;
    }
    if (strncmp(str, "Nov", 3) == 0)
    {
        return 0x11;
    }
    if (strncmp(str, "Dev", 3) == 0)
    {
        return 0x12;
    }
}
void http_updatetime()
{
    char buf[550];
    int len = 0;
    char httpbuf[] = "get\r\n";
    int sock = TcpSocket("bjtime.cn", 80); //"bjtime.cn"

    char weekday[5];
    int date;
    char month[5];
    int year;
    int hour;
    int minite;
    int second;

    RTC_DateTypeDef date_t;
    RTC_TimeTypeDef time_t;
    if (sock == -1)
    {
        return;
    }
    pwifi->pqueue->clear();
    TcpSend(sock, httpbuf, strlen(httpbuf));
    em_printf(">wait to get.\r\n");
    pwifi->pqueue->readAll(buf, &len, 3000);
    em_printf("len: %d\r\n", len);

    if (len != 0)
    {
        //em_printf(">Receive Data : %.*s.\r\n", len, buf);
        char *pstr = buf;
        while (strncmp("Date:", pstr, 5) != 0)
        {
            //em_printf(">Think %.*s.\r\n", 4, pstr);
            pstr++;
            //osDelay(20);
        }
        pstr += 6;
        // em_printf("\r\n-------------\r\n");
        // em_printf("\r\n>Think %.*s.\r\n", 20, pstr);
        HAL_RTC_GetTime(&hrtc, &time_t, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &date_t, RTC_FORMAT_BIN);

        sscanf(pstr, "%s %d %s %d %d:%d:%d", weekday, &date, month, &year, &hour, &minite, &second);
        hour += 8;
        second += 3;
        if (second > 60)
            second -= 60;
        em_printf(">%s %d %s %d %02d:%02d:%02d\r\n", weekday, date, month, year, hour, minite, second);

        date_t.WeekDay = m_str2weekday(weekday);
        date_t.Date = date;
        date_t.Year = year - 2000;
        date_t.Month = m_str2month(month);

        time_t.Hours = hour; //hour + 8 + 1;
        time_t.Minutes = minite;
        time_t.Seconds = second;

        HAL_RTC_SetTime(&hrtc, &time_t, RTC_FORMAT_BIN);
        HAL_RTC_SetDate(&hrtc, &date_t, RTC_FORMAT_BIN);

        // em_printf("\r\n test-> rtc 2s later\r\n");
        // osDelay(15000);

        // while (1)
        // {
        //     HAL_RTC_GetTime(&hrtc, &time_t, RTC_FORMAT_BIN);
        //     HAL_RTC_GetDate(&hrtc, &date_t, RTC_FORMAT_BIN);

        //     em_printf("%02d/%02d/%02d   %02d\r\n", 2000 + date_t.Year, date_t.Month, date_t.Date, date_t.WeekDay);
        //     em_printf("%02d:%02d:%02d\r\n", time_t.Hours, time_t.Minutes, time_t.Seconds);
        //     osDelay(1000);
        // }
    }
}