/*
 * @Author: your name
 * @Date: 2020-01-14 10:42:46
 * @LastEditTime : 2020-01-16 12:18:54
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \demo\MQTT\transport.c
 */
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

int transport_getdata(unsigned char *buf, int count)
{

	if ( pwifi->mqttqueue->read(buf, count, 120000) != wfOk )
	{
		return 0;
	}

	return count;
}
int transport_getdata_once(unsigned char *buf, int count)
{
	pwifi->mqttqueue->read(buf, count, 100);
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
