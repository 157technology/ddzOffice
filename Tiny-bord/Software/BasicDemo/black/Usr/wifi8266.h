#ifndef __WIFI8266_H__
#define __WIFI8266_H__ 

#include "wifi8266_config.h"

typedef struct tWifi8266Device
{
	char cmd[64];

    uint8_t is_active;

    /* basic command */
    WIFI_STATUS 	(*at) 					(wifi8266Device_t * this);
    WIFI_STATUS   	(*restart)				(void);
    WIFI_STATUS 	(*echo)					(ECHO_STATE s);

    /* wifi command */
    // station
    WIFI_STATUS		(*setWifiMode)			(WIFI_MODE mode);
    WIFI_STATUS		(*scanAP)				(void);
    WIFI_STATUS		(*joinAp)				(char * ssid, char * pwd);
    WIFI_STATUS		(*quitAP)				(void);

    WIFI_STATUS		(*smartConfigStart)		(void);
    WIFI_STATUS		(*smartConfigStop)		(void);

	// for AP mode
    //WIFI_STATUS		(*setAP)			(char * ssid, char * pwd, uint8_t chl, uint8_t ecn);

    /* tcp/ip */
    WIFI_STATUS		(*getIpStatus)			(void);
    WIFI_STATUS		(*ping)					(char * ipaddress);
    WIFI_STATUS 	(*getIp)				(void);
    WIFI_STATUS		(*domainIp)				(char * ipaddress);
    WIFI_STATUS		(*connect)				(char * mode, char * remoteIp, uint16_t remotePort);
    WIFI_STATUS 	(*disconnect)			(void);
    WIFI_STATUS 	(*sendStart)			(char * str);
    WIFI_STATUS		(*sendStop)				(void);

    WIFI_STATUS 	(*setTransLink)			(uint16_t mode, char * remoteIp, uint16_t remotePort);


	SerialDev_t * pWifiSerial;
}wifi8266Device_t;

#endif