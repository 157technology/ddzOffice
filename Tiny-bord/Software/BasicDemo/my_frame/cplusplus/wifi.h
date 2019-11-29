#ifndef __WIFI_H__
#define __WIFI_H__

#include "main.h"

typedef enum
{
	rtOk = 0,
	rtTimeOut,
	rtError
} rt_State_t;

typedef enum
{
	Wifi_Unactivate = 0,
	Wifi_Ok,
	Wifi_Error
} wifiState_t;

typedef enum
{
	Wifi_STA = 1,
	Wifi_AP,
	Wifi_APSTA
} WifiMode_t;

typedef struct link_s
{
	int  isServer;
	int  localPort;
	char LinkType[5];		// TCP / UDP / NONE
	char remoteIP[20];		// 远端 ip
	int  remotePort;		// 远端端口号
}link_t;

typedef struct fsm_s
{
	int hasWork;			// 是否工作     - can mcu communicate with wifi
	int hasIp;				// 是否获取IP   - connect tp ap and get ip
	int hasInternet;		// 是否联网     - connect to internet
	int hasLink;			// 是否建立链接 - already have connection
	int linkCount;			// 建立链接的数量
	char AP[32];
	char IP[20];
	char MAC[20];
	link_t * plinks;
} fsm_t;

typedef struct wifi_s
{
	uint16_t hasLock;		// 线程锁
	fsm_t *  status;		// 状态

	/*  */

	wifiState_t (*init)				();
	wifiState_t (*setMode) 			(WifiMode_t mode);
	wifiState_t (*scanAP)			();
	wifiState_t (*joinAP)			(char * ssd, char * psw);
	wifiState_t (*quitAP)			();
	wifiState_t (*ipconfig)			();
	wifiState_t (*ping)				(char * ip);
	wifiState_t (*dns)				(char * addr);

	wifiState_t (*tcp)				(char * remoteip, int remoteport);
	wifiState_t (*udp)				(char * remoteip, int remoteport);

	wifiState_t (*send)				(char * data);

	wifiState_t (*fsm)				();

	wifiState_t (*recieve)			(char * data);
} wifi_t;


extern wifi_t wifi;


void showStatus();

#endif