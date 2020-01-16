/*
 * @Author: your name
 * @Date: 2020-01-15 08:51:50
 * @LastEditTime : 2020-01-16 12:18:03
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \demo\EM_Device\wifi.h
 */
#ifndef __WIFI_H__
#define __WIFI_H__

#include "serial.h"
/*结构体里只存放数据 only data*/
typedef enum __wifistate
{
    InActive,
    NoWifi,
    Disnet,
    Internet
} WifiState;

typedef enum __wf
{
    wfOk,
    wfError,
    wfTimeOut
} WF;

typedef int socket;

typedef struct __wfqueuedata
{
    char *buf;
    char *cursor;
    char *read;
    int len;
    int lock;
} WF_QueueData;

typedef struct __wfqueue
{
    void (*clear)(void);
    void (*append)(char *data, int len);
    WF (*read)(char *aim, int len, int timeout);
    WF (*readAll)(char *aim, int *len, int timeout);

} WF_Queue;

typedef struct __wifi
{
    Serial *link;
    WifiState state;

    /*cmd mode<flag>*/
    int cmd;         // <1>commond return, <2>nocommod return
    int smartconfig; //special commond -- smart config

    /*net information*/
    int ipstatus; //ipstatus <2>..., <3>..., <4>..., <5>...
    int ping;     //ping time
    char AP[16];  //ap name
    char IP[16];  //local ap address
    char MAC[20]; //local mac address

    char BroadCast[16]; //broadcast addtess

    WF_Queue *pqueue;

    /*tcp/ip*/
    socket pool[5]; //socket pool:<0>unused, <1>used--max connect five
    socket sock_mqtt;

    /*read*/

    int mqtt_read;
    int mqtt_len;
    char *mqtt_data;
    // mqtt queue
    WF_Queue *mqttqueue;

} Wifi;

extern Wifi *pwifi;

Wifi *Wifi_Regester(UART_HandleTypeDef *puart, int rbufSize, int tbufSize);

WF WifiSmart();
WF WifiJoinAP(char *ssid, char *pwd);
WF WifiInit();
void WifiSelfCheck();

socket TcpSocket(char *ipaddr, int port);
WF TcpSend(socket sock, char *data, int len);

/**/
#define NetSend(data, len) emHAL_UART_Transmit_DMA(pwifi->link->puart, (uint8_t *)(data), (len))

#endif // !__WIFI_H__