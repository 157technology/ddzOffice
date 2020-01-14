/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/

#include "main.h"
#include "wifi.h"
#include "serial.h"
#include "commom.h"
#include <string.h>
#include <stdlib.h>

/**
This simple low-level implementation assumes a single connection for a single thread. Thus, a static
variable is used for that connection.
On other scenarios, the user must solve this by taking into account that the current implementation of
MQTTPacket_read() has a function pointer for a function call to get the data to a buffer, but no provisions
to know the caller or other indicator (the socket id): int (*getfn)(unsigned char*, int)
*/
//static int mysock = INVALID_SOCKET;

int transport_sendPacketBuffer(int sock, unsigned char *buf, int buflen)
{
	TcpSend(sock, (char *)buf, buflen);
	return buflen;
}

extern Wifi *wifi;
int transport_getdata(unsigned char *buf, int count)
{
	//int rc = recv(mysock, buf, count, 0);
	//printf("received %d bytes count %d\n", rc, (int)count);
	while ( wifi->mqtt_read == 0 )
	{
		//em_printf(">%d", wifi->mqtt_read);
		osDelay(5);
	}
	memcpy(buf, wifi->mqtt_data, count);
	//em_printf("%d->0x%x\n", wifi->mqtt_len, *wifi->mqtt_data);
	wifi->mqtt_data++;
	wifi->mqtt_len--;
	if ( wifi->mqtt_len <= 0 )
	{
		wifi->mqtt_read = 0;
	}
	//wifi->mqtt_read = 0;
	//return rc;
	return count;
}
int transport_getdata_once(unsigned char *buf, int count)
{
	//int rc = recv(mysock, buf, count, 0);
	//printf("received %d bytes count %d\n", rc, (int)count);
	while ( wifi->mqtt_read == 0 )
	{
		//em_printf(">%d", wifi->mqtt_read);
		//osDelay(5);
		memset(buf, 0, count);
		return count;
	}
	memcpy(buf, wifi->mqtt_data, count);
	//em_printf("%d->0x%x\n", wifi->mqtt_len, *wifi->mqtt_data);
	wifi->mqtt_data++;
	wifi->mqtt_len--;
	if ( wifi->mqtt_len <= 0 )
	{
		wifi->mqtt_read = 0;
	}
	//wifi->mqtt_read = 0;
	//return rc;
	return count;
}

int transport_getdatanb(void *sck, unsigned char *buf, int count)
{
	return 0;
}

/**
return >=0 for a socket descriptor, <0 for an error code
@todo Basically moved from the sample without changes, should accomodate same usage for 'sock' for clarity,
removing indirections
*/
//${YourProductKey}.iot-as-mqtt.${YourRegionId}.aliyuncs.com:1883

int transport_open(char *addr, int port)
{
	return TcpSocket(addr, port);
}

int transport_close(int sock)
{

	return 0;
}
