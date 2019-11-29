#include "wifi.h"
#include "wifi_config.h"

/**/
static rt_State_t m_get_ping(int timeout, char * str, char * dst);
static rt_State_t m_get_ip(int timeout, char * str, char * dst);
static rt_State_t m_get_ap(int timeout, char * str, char * dst);

static rt_State_t refresh(int timeout, char * str);
static rt_State_t check(int timeout, char * str);
static rt_State_t AT();
static void QuitPassThrough();


/**/
static wifiState_t wifi_init();
static wifiState_t wifi_set_mode(WifiMode_t mode);
static wifiState_t wifi_scan_AP();
static wifiState_t wifi_join_AP(char * ssid, char * pwd);
static wifiState_t wifi_quit_AP();
static wifiState_t wifi_ipconfig();
static wifiState_t wifi_ping(char * ip);
static wifiState_t wifi_dns(char * addr);
static wifiState_t wifi_tcp(char * addr, int port);
static wifiState_t wifi_udp(char * addr, int port);
static wifiState_t wifi_send(char * data);

static wifiState_t wifi_FSM(void);

static wifiState_t wifi_recieve(char * str);

void showStatus();

static link_t m_link[5] = {
	{0, 0, "", "", 0},
	{0, 0, "", "", 0},
	{0, 0, "", "", 0},
	{0, 0, "", "", 0},
	{0, 0, "", "", 0},
};

static fsm_t m_fsm = {
	0,
	0,
	0,
	0,
	0,
	"",
	"",
	"",
	m_link,
};

wifi_t wifi = {
	0,
	&m_fsm,
	wifi_init,
	wifi_set_mode,
	wifi_scan_AP,
	wifi_join_AP,
	wifi_quit_AP,
	wifi_ipconfig,
	wifi_ping,
	wifi_dns,

	wifi_tcp,
	wifi_udp,
	wifi_send,

	wifi_FSM,
	wifi_recieve,
};
static rt_State_t m_get_ping(int timeout, char * str, char * dst)
{
	SerialDev_t * serial = wifiDev;
	char error[] = "ERROR\r\n";
	int count = 0;
	int ping = 0;
	while ( 1 )
	{
		count ++;
		osDelay(20);

		if ( serial->rCnt && serial->rflag )
		{
			serial->rflag = 0;
			//判断数据
			//WIFI_DEBUG(">>>RT :: %s, %d\n", serial->rbuf, serial->rCnt);

			sscanf(serial->rbuf, "+%d", &ping);


			if ( strcmp(serial->rbuf + serial->rCnt - strlen(str), str) == 0 )
			{
				//WIFI_DEBUG("RT:: SAME\n");
				WIFI_DEBUG(">>ping:: %d\n", ping);
				m_fsm.hasInternet = 1;
				return rtOk;
			}
			if ( strcmp(serial->rbuf + serial->rCnt - strlen(error), error) == 0 )
			{
				WIFI_DEBUG("RT:: error\n");
				m_fsm.hasInternet = 0;
				return rtOk;
			}
			//return rtOk;
		}

		if ( count  > timeout / 20 )
		{
			WIFI_DEBUG("RT:: TIMEOUT\n");
			return rtTimeOut;// time out
		}
	}
}
static rt_State_t m_get_ip(int timeout, char * str, char * dst)
{
	SerialDev_t * serial = wifiDev;
	int count = 0;
	char ip[20];
	char mc[20];
	while ( 1 )
	{
		count ++;
		osDelay(20);

		if ( serial->rCnt && serial->rflag )
		{
			serial->rflag = 0;
			//判断数据
			//WIFI_DEBUG(">>>RT :: %s, %d\n", serial->rbuf, serial->rCnt);

			sscanf(serial->rbuf, "+CIFSR:STAIP,%s\r\n+CIFSR:STAMAC,%s ", ip, mc);

			//过滤掉 "
			ip[strlen(ip) - 1] = '\0';
			mc[strlen(mc) - 1] = '\0';

			memcpy(m_fsm.IP, ip + 1, strlen(ip) - 1);
			memcpy(m_fsm.MAC, mc + 1, strlen(mc) - 1);
			// WIFI_DEBUG(">>ip:: %s\n>>mc:: %s\n", ip + 1, mc + 1);
			// WIFI_DEBUG(">>ip:: %s\n>>mc:: %s\n", m_fsm.IP, m_fsm.MAC);

			if ( strcmp(serial->rbuf + serial->rCnt - strlen(str), str) == 0 )
			{

				return rtOk;
			}

			//return rtOk;
		}

		if ( count  > timeout / 20 )
		{
			WIFI_DEBUG("RT:: TIME OUT\n");
			return rtTimeOut;// time out
		}
	}
}
static rt_State_t m_get_ap(int timeout, char * str, char * dst)
{
	SerialDev_t * serial = wifiDev;
	int count = 0;
	char ap[10];
	while ( 1 )
	{
		count ++;
		osDelay(20);

		if ( serial->rCnt )
		{

			//WIFI_DEBUG(">>>RT :: %s, %d\n", serial->rbuf, serial->rCnt);

			//sscanf(serial->rbuf, "+CWJAP:\"%s\",\"", ap);
			//判断数据
			int len = strlen("+CWJAP:");
			if ( strncmp(serial->rbuf, "+CWJAP:", len) == 0 )
			{
				//提取 AP name
				int begin = 0;
				int pos = 0;
				for ( int i = len; i < serial->rCnt; i ++ )
				{
					if ( serial->rbuf[i] == '"' )
					{
						if ( begin == 0 )
						{
							begin = 1;
						}
						else
						{
							ap[pos++] = '\0';
							memcpy(m_fsm.AP, ap, pos);// m_fsm.AP
							break;
						}
					}
					else	ap[pos++] = serial->rbuf[i];
				}
			}




			WIFI_DEBUG(">>ap name:: %s\n", m_fsm.AP);

			if ( strcmp(serial->rbuf + serial->rCnt - strlen(str), str) == 0 )
			{

				return rtOk;
			}

			//return rtOk;
		}

		if ( count  > timeout / 20 )
		{
			WIFI_DEBUG("RT:: TIME OUT\n");
			return rtTimeOut;// time out
		}
	}
}
static rt_State_t refresh(int timeout, char * str)
{
	SerialDev_t * serial = wifiDev;
	int count = 0;
	static int s = 0, ls = 0;
	char sss[64];
	while ( 1 )
	{
		count ++;
		osDelay(20);

		if ( serial->rCnt && serial->rflag )
		{
			serial->rflag = 0;
			//判断数据
			//WIFI_DEBUG(">>>RT :: %s, %d\n", serial->rbuf, serial->rCnt);

			sscanf(serial->rbuf, "STATUS:%d", &s);
			if ( s == ls && s != 3 )
			{
				return rtOk;
			}
			ls = s;
			//WIFI_DEBUG(">>State:: %d\n", s);
			if ( s == 2 )
			{
				print("AT+CWJAP?\r\n");
				m_get_ap(1000, "OK\r\n", NULL);
				//查询 本机 IP MAC
				print("AT+CIFSR\r\n");
				m_get_ip(1000, "OK\r\n", NULL);
				wifi.status->hasIp = 1;
				m_fsm.linkCount = 0;
			}
			else if ( s == 3 )
			{
				wifi.status->hasIp = 1;
				wifi.status->hasIp   = 1;
				wifi.status->hasLink = 1;
				char * pstr = serial->rbuf + 10;

				int m_cnt = 0;
				while ( 1 )
				{
					sss[0] = '\0';
					//WIFI_DEBUG("str>>> %s\n", pstr);
					sscanf(pstr, "+CIPSTATUS:%s", sss);
					//WIFI_DEBUG("sss>>> %s\n", sss);
					if ( strcmp(sss, "") == 0 )		break;
					pstr = pstr + 11 + 2 + strlen(sss);
					for ( int i = 0; i < strlen(sss); i ++ )
					{
						if ( sss[i] == ',' || sss[i] == '"' )	sss[i] = ' ';
					}
					int server;
					sscanf(sss, "%d %s %s %d %d", &m_link[m_cnt].isServer, m_link[m_cnt].LinkType, m_link[m_cnt].remoteIP, &m_link[m_cnt].remotePort, &m_link[m_cnt].localPort);
					// WIFI_DEBUG(">>Link:: %s\n", m_link[m_cnt].LinkType);
					// WIFI_DEBUG(">>Link:: %s\n", m_link[m_cnt].remoteIP);
					// WIFI_DEBUG(">>Link:: %d\n", m_link[m_cnt].remotePort);
					m_cnt++;
				}
				m_fsm.linkCount = m_cnt;
				//WIFI_DEBUG("cnt-->%d\n", m_cnt);

			}
			else if ( s == 4 )
			{
				print("AT+CWJAP?\r\n");
				m_get_ap(1000, "OK\r\n", NULL);
				//查询 本机 IP MAC
				print("AT+CIFSR\r\n");
				m_get_ip(1000, "OK\r\n", NULL);
				wifi.status->hasIp   = 1;
				m_fsm.linkCount = 0;
			}
			else if ( s == 5 )
			{
				wifi.status->hasIp   = 0;
				m_fsm.linkCount = 0;
				WIFI_DEBUG("NO AP\n");
			}
			else
			{
				WIFI_DEBUG("UNDEFINE status\n");
			}

			if ( strcmp(serial->rbuf + serial->rCnt - strlen(str), str) == 0 )
			{
				//WIFI_DEBUG("RT:: SAME\n");
				return rtOk;
			}

			//return rtOk;
		}

		if ( count  > timeout / 20 )
		{
			WIFI_DEBUG("RT:: TIME OUT\n");
			return rtTimeOut;// time out
		}
	}
}

/**
 * 判断接收返回数据
 * @Author   EMei_Li
 * @DateTime 2019-11-19T16:20:28+0800
 * @param    timeout                  等待接收超时时间
 * @param    str                      字符串匹配
 * @return                            [description]
 */
static rt_State_t check(int timeout, char * str)
{
	SerialDev_t * serial = wifiDev;
	int count = 0;
	while ( 1 )
	{
		count ++;
		osDelay(20);

		if ( serial->rCnt && serial->rflag )
		{
			serial->rflag = 0;
			//WIFI_DEBUG(serial->rbuf);
			if ( strcmp(serial->rbuf + serial->rCnt - strlen(str), str) == 0 )
			{
				//WIFI_DEBUG("RT:: SAME\n");
				return rtOk;
			}
		}

		if ( count  > timeout / 20 )
		{
			return rtTimeOut;// time out
		}
	}
}


static rt_State_t AT()
{
	print("AT\r\n");
	return check(500, "OK\r\n");
}
static void QuitPassThrough()
{
	print("+++");
	osDelay(20);
}

/**
 * [wifi_init description]
 * @Author   EMei_Li
 * @DateTime 2019-11-22T16:24:51+0800
 * @return   [description]
 */
static wifiState_t wifi_init()
{
	WIFI_DEBUG("wifi_init start\n");
	rt_State_t rt = AT();
	WIFI_DEBUG("wifi_init at\n");
	if ( rt == rtOk )
	{
		print("ATE0\r\n");
		wifi.status->hasWork = 1;
		osDelay(50);

		print("AT+CIPMUX=1\r\n");
		check(500, "OK\r\n");

		wifi.fsm();
		WIFI_DEBUG("wifi_init fsm\n");

		if ( wifi.status->hasIp )
		{
			//查询 ap name
			print("AT+CWJAP?\r\n");
			m_get_ap(1000, "OK\r\n", NULL);

			//查询 本机 IP MAC
			print("AT+CIFSR\r\n");
			m_get_ip(1000, "OK\r\n", NULL);

			//测试是否联网
			print("AT+PING=\"%s\"\r\n", "www.baidu.com");
			m_get_ping(10000, "OK\r\n", NULL);

			//show status
			showStatus();

		}
		WIFI_DEBUG("wifi_init end\n");
		return Wifi_Ok;
	}
	else if (rt == rtError)
	{
		WIFI_DEBUG("wifi_init error\n");
		return Wifi_Error;
	}
	else
	{	//time out
		QuitPassThrough();
		rt = AT();
		if ( rt == rtOk )
		{
			print("ATE0\r\n");

			return Wifi_Ok;
		}
		else if (rt == rtError)
		{
			return Wifi_Error;
		}
		else//time out
		{
			WIFI_DEBUG("Please check wire connection");
			return Wifi_Unactivate;
		}
	}
	WIFI_DEBUG("wifi_init undefine error\n");
}

static wifiState_t wifi_set_mode(WifiMode_t mode)
{


	print("AT+CWMODE=%d\r\n", mode);
	return check(2000, "OK\r\n");
}
/**
 * [wifi_set_mode  description]
 * @Author   EMei_Li
 * @DateTime 2019-11-19T22:40:42+0800
 * @return   [description]
 */
static wifiState_t wifi_scan_AP()
{


	print("AT+CWLAP\r\n");
	return check(20000, "OK\r\n");
}
/**
 * [wifi_join_AP description]
 * @Author   EMei_Li
 * @DateTime 2019-11-20T09:53:03+0800
 * @param    ssid                     [description]
 * @param    pwd                      [description]
 * @return                            [description]
 */
static wifiState_t wifi_join_AP(char * ssid, char * pwd)
{
	//wifi.hasIp = 0;



	print("AT+CWJAP_DEF=\"%s\",\"%s\"\r\n", ssid, pwd);

	rt_State_t rt = check(20000, "OK\r\n");

	if ( rt == rtOk )
		return rt;
}

/**
 * [wifi_quit_AP description]
 * @Author   EMei_Li
 * @DateTime 2019-11-20T09:57:09+0800
 * @return   [description]
 */
static wifiState_t wifi_quit_AP()
{
	print("AT+CWQAP\r\n");
	rt_State_t rt = check(20000, "OK\r\n");
	if ( rt == rtOk )
		return rt;
}

static wifiState_t wifi_ipconfig()
{
	//

	print("AT+CIFSR\r\n");
	return check(5000, "OK\r\n");
}

/**
 * [wifi_ping description]
 * @Author   EMei_Li
 * @DateTime 2019-11-20T10:18:55+0800
 * @param    ip                       [description]
 * @return                            [description]
 */
static wifiState_t wifi_ping(char * ip)
{


	print("AT+PING=\"%s\"\r\n", ip);
	return check(5000, "OK\r\n");
}

static wifiState_t wifi_dns(char * addr)
{


	print("AT+CIPDOMAIN=\"%s\"\r\n", addr);
	return check(5000, "OK\r\n");
}

static wifiState_t wifi_tcp(char * addr, int port)
{

	print("AT+CIPSTART=0,\"TCP\",\"%s\",%d\r\n", addr, port);
	return check(5000, "OK\r\n");
}

static wifiState_t wifi_udp(char * addr, int port)
{

	print("AT+CIPSTART=1,\"UDP\",\"%s\",%d\r\n", addr, port);
	return check(5000, "OK\r\n");
}

static wifiState_t wifi_send(char * data)
{

	if ( 1 )
	{
		// normal
		print("AT+CIPSEND=1,%d\r\n", strlen(data));
		check(5000, "OK\r\n> ");
		//WIFI_DEBUG("---");
		print(data);
		return check(5000, "OK\r\n");
	}
	else
	{

	}
}

/**
 * wifi8266 状态机, 在wifi状态机线程中调用
 * @Author   EMei_Li
 * @DateTime 2019-11-22T09:13:53+0800
 * @return                            [description]
 */
static wifiState_t wifi_FSM(void)
{
	static int cnt = 0;
	if ( wifi.hasLock == 1 )
	{
		WIFI_DEBUG("> Wifi is used.\n");
		return Wifi_Error;
	}
	wifi.hasLock = 1;

	print("AT+CIPSTATUS\r\n");

	refresh(5000, "OK\r\n");

	if ( cnt ++ == 10 )
	{
		cnt = 0;
		print("AT+PING=\"%s\"\r\n", "www.baidu.com");
		m_get_ping(10000, "OK\r\n", NULL);
	}

	wifi.hasLock = 0;
}


static wifiState_t wifi_cloud()
{
	//check

	return wifi_tcp("183.230.40.40", 1811);
}

/**
 * [wifi_recieve description]
 * @Author   EMei_Li
 * @DateTime 2019-11-22T16:20:46+0800
 * @param    str                      [description]
 * @return                            [description]
 */
static wifiState_t wifi_recieve(char * str)
{
	//识别帧头
	if ( strlen(str) < 6 )	return Wifi_Ok;

	int linkID, len;
	if ( strncmp(str, "\r\n+IPD", 6) == 0 )
	{
		str += 7;
		
		sscanf(str, "%d,%d", &linkID, &len);
		while ( (*str) != ':' )	str ++;
		str ++;
		WIFI_DEBUG("link: %d.\nlen: %d\n", linkID, len);
		WIFI_DEBUG("%s\n",str);
	}

}

void showStatus()
{
	WIFI_DEBUG("\n*******************************\n");
	WIFI_DEBUG("**********wifi status**********\n");
	WIFI_DEBUG("WLAN 	   | %-20s\n", m_fsm.AP);
	WIFI_DEBUG("IP   	   | %-20s\n", m_fsm.IP);
	WIFI_DEBUG("MAC  	   | %-20s\n", m_fsm.MAC);
	WIFI_DEBUG("Internet   | %-20s\n", m_fsm.hasInternet ? "CONNECT" : "DIS-CONNECT");
	for ( int i = 0; i < m_fsm.linkCount; i ++ )
	{
		WIFI_DEBUG("-------------------------------\n");
		WIFI_DEBUG("LinkType   | %-20s\n", m_link[i].LinkType);
		WIFI_DEBUG("remoteIP   | %-20s\n", m_link[i].remoteIP);
		WIFI_DEBUG("remotePort | %-20d\n", m_link[i].remotePort);
		WIFI_DEBUG("LocalPort  | %-20d\n", m_link[i].localPort);
	}
	WIFI_DEBUG("*******************************\n");
}