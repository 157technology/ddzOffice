#include "wifi.h"

#include "serial.h"
#include "usart.h"
#include "cmsis_os2.h"
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
Wifi *pwifi;
/*###########################################*/
void slot_link_hasData(void *data);
/*###########################################*/

/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

Wifi *Wifi_Regester(UART_HandleTypeDef *puart, int rbufSize, int tbufSize)
{
    Wifi *wifi = (Wifi *)MALLOC(sizeof(Wifi));

    // 初始化通信串口 | 设置槽函数
    wifi->link = Serial_Regester(puart, rbufSize, tbufSize);

    connect(wifi->link->signal, slot_link_hasData, DirectConnect);
    // 初始化wifi的状态
    wifi->state = InActive;
    wifi->cmd = 0;
    wifi->smartconfig = 0;
    wifi->mqtt_read = 0;
    memset(wifi->pool, 0, sizeof(wifi->pool)); //socket unuse
    // link to local
    pwifi = wifi;
    return wifi;
}

void slot_link_hasData(void *data)
{
    char *str = (char *)data;
    em_printf("wifi have data %d.\n", pwifi->link->rCnt);
    em_printf("%s", str);
    if (pwifi->cmd)
    {
        //em_printf(">>>cmd# %s.\n", str);
        if (pwifi->smartconfig == 1)
        {
            //处理smartconfig返回的信息
            //em_printf(">>>cmd smart# %s.\n", str);
            if (strcmp(str, "smartconfig connected wifi\r\n") == 0)
            {
                //配网成功
                pwifi->smartconfig = 0;
            }
        }
    }
    else
    {
        //em_printf(">>>wifi# %s.\n", str);
        int num = pwifi->link->rCnt;
        while (num > 5)
        {
            if (strncmp(str, "+IPD", 4) == 0)
            {
                int m_sock, m_len; //获取到网络信息, 解析
                sscanf(str + 5, "%d,%d", &m_sock, &m_len);
                //em_printf("sock:%d, len:%d", m_sock, m_len);
                if (m_sock == pwifi->sock_mqtt)
                {
                    //是mqtt的信息
                    //em_printf("获取到mqtt的数据\n");
                    while (*str != ':')
                        str++;
                    str++;
                    pwifi->mqtt_read = 1;
                    pwifi->mqtt_data = str;
                    pwifi->mqtt_len = m_len;
                    // for (int i = 0; i < m_len; i++)
                    //     em_printf("0x%X ", str[i]);
                    // em_printf("\n");
                }
                break;
            }
            else
            {
                str++;
                num--;
            }
            /* code */
        }
        /* code */
    }
}

static void cb_ipstatus(void *data)
{
    char *str = (char *)data;

    sscanf(str, "STATUS:%d", &pwifi->ipstatus);
}
static void cb_apinfo(void *data)
{
    char *str = (char *)data;
    while (*str != '\0')
    {
        if (*str == ',' || *str == '"')
            *str = ' ';
        str++;
    }
    sscanf((char *)data, "+CWJAP: %s", pwifi->AP);
}
static void cb_ipinfo(void *data)
{
    char *str = (char *)data;
    while (*str != '\0')
    {
        if (*str == ',' || *str == '"')
            *str = ' ';
        str++;
    }
    sscanf((char *)data, "+CIFSR:STAIP %s +CIFSR:STAMAC %s", pwifi->IP, pwifi->MAC);
}

static void cb_ping(void *data)
{
    char *str = (char *)data;
    if (str[0] != '+')
        return;
    while (*str != '\0')
    {
        if (*str == ',' || *str == '"')
            *str = ' ';
        str++;
    }
    sscanf((char *)data, "+%d", &pwifi->ping);
}

static void cb_joinap(void *data)
{
    char *str = (char *)data;
    if (str[0] != '+')
        return;
    while (*str != '\0')
    {
        if (*str == ',' || *str == '"')
            *str = ' ';
        str++;
    }
    //sscanf((char *)data, "+%d", &m_wifi->ping);
}

static WF command_at_once(char *cmd, char *check, int timeout)
{
    //set state
    pwifi->cmd = 1;
    pwifi->link->rCnt = 0;

    // send cmd
    Serial_Print(pwifi->link, "%s\r\n", cmd);

    for (int i = 0; i < timeout / 20; i++)
    {
        // 查询接收
        if (pwifi->link->rCnt != 0)
        {
            char *str = pwifi->link->rbuf;

            // check <str, check>
            if (strcmp(str + strlen(str) - strlen(check), check) == 0)
            {
                pwifi->cmd = 0;
                return wfOk;
            }
            if (strlen(str) > strlen("ERROR\r\n") && strcmp(str + strlen(str) - strlen("ERROR\r\n"), "ERROR\r\n") == 0)
            {
                pwifi->cmd = 0;
                return wfError;
            }
            if (strlen(str) > strlen("FAIL\r\n") && strcmp(str + strlen(str) - strlen("FAIL\r\n"), "FAIL\r\n") == 0)
            {
                pwifi->cmd = 0;
                return wfError;
            }
            if (strlen(str) > strlen("ALREADY CONNECT\r\n") && strcmp(str + strlen(str) - strlen("ALREADY CONNECT\r\n"), "ALREADY CONNECT\r\n") == 0)
            {
                pwifi->cmd = 0;
                return wfError;
            }
            // Error
            //clear
            pwifi->link->rCnt = 0;
        }

        // delay
        osDelay(20);
    }
    return wfTimeOut;
}

static WF command_at_func(char *cmd, char *check, pfun cb, int timeout)
{
    //set state
    pwifi->cmd = 1;
    pwifi->link->rCnt = 0;

    // send cmd
    Serial_Print(pwifi->link, "%s\r\n", cmd);

    for (int i = 0; i < timeout / 20; i++)
    {
        // 查询接收
        if (pwifi->link->rCnt != 0)
        {
            char *str = pwifi->link->rbuf;

            //回调函数
            cb((void *)str);
            // check <str, check>
            if (strcmp(str + strlen(str) - strlen(check), check) == 0)
            {
                pwifi->cmd = 0;
                return wfOk;
            }
            // Error
            if (strlen(str) > strlen("ERROR\r\n") && strcmp(str + strlen(str) - strlen("ERROR\r\n"), "ERROR\r\n") == 0)
            {
                pwifi->cmd = 0;
                return wfError;
            }
            if (strlen(str) > strlen("FAIL\r\n") && strcmp(str + strlen(str) - strlen("FAIL\r\n"), "FAIL\r\n") == 0)
            {
                pwifi->cmd = 0;
                return wfError;
            }
            //clear
            pwifi->link->rCnt = 0;
        }

        // delay
        osDelay(20);
    }
    return wfTimeOut;
}

WF WIFI_AT_CMD()
{
    //AT 测试
    // timeout:100 ms
    return command_at_once("AT", "OK\r\n", 100);
}
WF WIFI_AT_RST()
{
    //AT 测试
    // timeout:100 ms
    // at once -> OK
    if (command_at_once("AT+RST", "OK\r\n", 100) != wfOk)
    {
        // error
        //return wfError;
    }
    //wait to restart
    pwifi->cmd = 1;
    pwifi->link->rCnt = 0; // set flag
                            // set two seconds to wait to start
                            // then    -> ready
    for (int i = 0; i < 100; i++)
    {
        if (pwifi->link->rCnt != 0)
        {
            char *str = pwifi->link->rbuf;

            if (strlen(str) > strlen("ready\r\n") && strcmp(str + strlen(str) - strlen("ready\r\n"), "ready\r\n") == 0)
            {
                pwifi->cmd = 0;
                return wfOk;
            }

            pwifi->link->rCnt = 0;
        }
        osDelay(20);
    }

    pwifi->cmd = 0;
    return wfTimeOut;
}
WF WIFI_AT_ATE()
{
    //关闭回显
    // timeout:100 ms
    return command_at_once("ATE0", "OK\r\n", 100);
}
WF WIFI_AT_IPSTATUS()
{
    // 查询网络连接信息
    // timeout:100 ms
    return command_at_func("AT+CIPSTATUS", "OK\r\n", cb_ipstatus, 100);
}
WF WIFI_AT_APINFO()
{
    // 查询AP信息
    // timeout:100 ms
    return command_at_func("AT+CWJAP?", "OK\r\n", cb_apinfo, 100);
}
WF WIFI_AT_IPINFO()
{
    // 查询IP信息
    // timeout:100 ms
    return command_at_func("AT+CIFSR", "OK\r\n", cb_ipinfo, 100);
}

WF WIFI_AT_PING()
{
    // Ping 测试
    // timeout:10000 ms
    return command_at_func("AT+PING=\"www.baidu.com\"", "OK\r\n", cb_ping, 10000);
}

WF WIFI_AT_TTT()
{
    //关闭上电自动连接
    // timeout:100 ms
    return command_at_once("AT+CWAUTOCONN=0", "OK\r\n", 100);
}

/**/
WF WifiJoinAP(char *ssid, char *pwd)
{
    //连接AP
    // timeout:20000 ms
    char str[64];
    sprintf(str, "AT+CWJAP_CUR=\"%s\",\"%s\"", ssid, pwd);
    return command_at_func(str, "OK\r\n", cb_joinap, 20000);
}

WF WifiSmart()
{
    //连接AP
    // timeout:20000 ms
    em_printf("start smart config.\n");
    if (command_at_once("AT+CWAUTOCONN=1", "OK\r\n", 100) == wfOk)
    {
        em_printf("AT+CWAUTOCONN=1.\n");
    }
    //断开当前连接
    if (command_at_once("AT+CWQAP", "OK\r\n", 100) == wfOk)
    {
        em_printf("quit ap.\n");
    }
    if (command_at_once("AT+CWSTARTSMART", "OK\r\n", 100) == wfOk)
    {
        em_printf("in start smart config.\n");
        pwifi->cmd = 1;
        pwifi->smartconfig = 1;
        while (pwifi->smartconfig)
        {
            osDelay(5000);
            em_printf(".");
        }
        em_printf("\n");
        if (command_at_once("AT+CWSTOPSMART", "OK\r\n", 100) == wfOk)
        {
            em_printf("stop smart\n");
            command_at_once("AT+CWAUTOCONN=0", "OK\r\n", 100);
        }
        return wfOk;
    }
    return wfError;
}

WF WifiInit()
{
    em_printf("\n# test ok\n");
    if (WIFI_AT_CMD() != wfOk) //AT test
    {
        return wfError;
    }
    em_printf("\n# reset\n");
    if (WIFI_AT_RST() != wfOk) //reset
    {
        em_printf("ERROR# reset\n");
        return wfError;
    };
    em_printf("\n# no echo\n");
    if (WIFI_AT_ATE() != wfOk) //no echo
    {
        return wfError;
    };
    em_printf("\n# wifi mode\n");
    if (command_at_once("AT+CWMODE_DEF=1", "OK\r\n", 100) != wfOk)
    {
        return wfError;
    }
    em_printf("\n# ipmux\n");
    //set ipmux <0>singlelink, <1>muxlink
    if (command_at_once("AT+CIPMUX=1", "OK\r\n", 100) != wfOk)
    {
        return wfError;
    }

    //
    // try to join a default ap
    // if not success then start smartconfig
    em_printf("\n# Join wifi\n");
    if (WifiJoinAP("emeili", "88888888") != wfOk)
    {
        if (WifiSmart() != wfOk)
        {
            return wfError;
        }
    }
    //end
    return wfOk;
}

void WifiSelfCheck()
{
    if (WIFI_AT_CMD() == wfOk) //AT test
    {
        pwifi->state = NoWifi;
        //should reset<-v->
        WIFI_AT_ATE();
        //WIFI_AT_TTT();
        //ap
        if (WIFI_AT_IPSTATUS() == wfOk)
        {
            if (pwifi->ipstatus == 5)
            {
                //NO AP
            }
            else
            {
                //AP
                if (WIFI_AT_APINFO() == wfOk)
                {
                    em_printf("root>>> AP: %s\n", pwifi->AP);
                }

                //IP
                if (WIFI_AT_IPINFO() == wfOk)
                {
                    em_printf("root>>>  IP: %s\n", pwifi->IP);
                    em_printf("root>>> MAC: %s\n", pwifi->MAC);
                }
                //PING

                if (WIFI_AT_PING() == wfOk)
                {
                    em_printf("root>>>Ping: %d\n", pwifi->ping);
                }
                else
                {
                    em_printf("no internet.\n");
                }

                if (pwifi->ipstatus == 3)
                {
                    //连接情况
                }
            }
        }
    }
}

void WifiShowStatus()
{
    //wifi mode

    //ap

    // ip mac

    // ping
}
/*##############################################*/

/*##############################################*/
socket TcpSocket(char *ipaddr, int port)
{
    char str[64];
    socket sock = -1; //获取一个socket<一共有five> -- sockct pool

    for (int i = 0; i < 5; i++)
    {
        if (pwifi->pool[i] == 0)
        {
            sock = i;
            break;
        }
    }
    if (sock == -1)
    {
        return -1;
    }

    sprintf(str, "AT+CIPSTART=%d,\"TCP\",\"%s\",%d", sock, ipaddr, port);
    if (command_at_once(str, "OK\r\n", 500) == wfOk)
    {
        //占用这个socket,并返回
        pwifi->pool[sock] = 1;
        return sock;
    }
    return -1;
}

// 255 广播地址 <xxx.xxx.xxx.255>
socket UdpSocket(char *ipaddr, int port)
{
    char str[64];
    socket sock = 1; //获取一个socket<一共有five> -- sockct pool
    sprintf(str, "AT+CIPSTART=%d,\"UDP\",\"%s\",%d", sock, ipaddr, port);
    if (command_at_once(str, "OK\r\n", 500) == wfOk)
    {
        //占用这个socket,并返回
        return sock;
    }
    return -1;
}

//data需要转化为字符串 +'\0'
//max(len) -- 2048
WF TcpSend(socket sock, char *data, int len)
{
    char str[64];
    sprintf(str, "AT+CIPSEND=%d,%d", sock, len);
    if (command_at_once(str, "> ", 100) == wfOk)
    {
        //开始发送 len
        // em_printf("start send:\n");
        // for (int i = 0; i < len; i++)
        // {
        //     em_printf("%c", data[i]);
        // }
        // em_printf("END..\n");
        //osDelay(1000);
        //HAL_UART_Transmit(m_wifi->link->puart, data, len, 9999);
        emHAL_UART_Transmit_DMA(pwifi->link->puart, (uint8_t *)data, len);
        //Serial_Print(m_wifi->link, "%s", data);

        //ckech if send ok
        {
        }
    }
}
