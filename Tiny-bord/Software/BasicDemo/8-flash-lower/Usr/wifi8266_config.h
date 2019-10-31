#ifndef __WIFI8266_CONFIG_H__
#define __WIFI8266_CONFIG_H__

#include "serial.h"


typedef enum
{
	CIP_NORMAL = 0,
	CIP_TRANSLINK
} WIFI_CIPMODE;

typedef enum
{
	ECHO_ON = 0,
	ECHO_OFF
} ECHO_STATE;

typedef enum
{
	WIFI_STATION = 1,
	WIFI_SOFTAP,
	WIFI_BOTHMODE
} WIFI_MODE;

typedef enum
{
	WIFI_OK = 0,
	WIFI_TIMEOUT,
	WIFI_PWDERROR,
	WIFI_NOAP,
	WIFI_CONNECTFAILED
} WIFI_STATUS;

#endif