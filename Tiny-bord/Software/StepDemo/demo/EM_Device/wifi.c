#include "wifi.h"

#include "serial.h"
#include "usart.h"
#include "cmsis_os2.h"
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
Wifi *pwifi;
/*###########################################*/
void slot_link_hasData(void *data);
/*###########################################*/
/* 队列实现 */
// void (*clear)(void);
// void (*append)(char *data, int len);
// void (*read)(char *aim, int len, int timeout);
// void (*readAll)(char *aim, int timeout);
static void WF_Queue_Clear(void);
static void WF_Queue_Append(char *data, int len);
static WF WF_Queue_Read(char *aim, int len, int timeout);
static WF WF_Queue_ReadAll(char *aim, int *len, int timeout);

WF_QueueData m_queue_data;

static void WF_Queue_Clear(void)
{
    while (m_queue_data.lock == 1)
    {
        osDelay(1);
    }
    m_queue_data.lock = 1;

    m_queue_data.cursor = m_queue_data.buf;
    m_queue_data.read = m_queue_data.buf;

    m_queue_data.len = 0;

    m_queue_data.lock = 0;
}

static void WF_Queue_Append(char *data, int len)
{
    while (m_queue_data.lock == 1)
    {
        osDelay(1);
    }
    m_queue_data.lock = 1;
    //if ( len > 512 )    len = 512;
    memcpy(m_queue_data.cursor, data, len);
    m_queue_data.cursor += len;
    m_queue_data.len += len;

    m_queue_data.lock = 0;
}

static WF WF_Queue_Read(char *aim, int len, int timeout)
{
    int count = 0;
    while (m_queue_data.lock == 1)
    {
        osDelay(1);
    }
    m_queue_data.lock = 1;

    while (len > m_queue_data.len)
    {
        m_queue_data.lock = 0;
        osDelay(1);
        if (++count >= timeout)
        {
            //time out
            m_queue_data.lock = 0;
            return wfTimeOut;
        }
    }

    memcpy(aim, m_queue_data.read, len);

    m_queue_data.read += len;
    m_queue_data.len -= len;

    m_queue_data.lock = 0;
    return wfOk;
}

static WF WF_Queue_ReadAll(char *aim, int *len, int timeout)
{
    int count = 0;
    while (m_queue_data.lock == 1)
    {
        osDelay(1);
    }
    m_queue_data.lock = 1;

    while (m_queue_data.len == 0)
    {
        m_queue_data.lock = 0;
        osDelay(1);
        if (++count >= timeout)
        {
            //time out
            m_queue_data.lock = 0;
            return wfTimeOut;
        }
    }

    *len = m_queue_data.len;
    memcpy(aim, m_queue_data.read, m_queue_data.len);

    m_queue_data.cursor = m_queue_data.buf;
    m_queue_data.read = m_queue_data.buf;
    m_queue_data.len = 0;

    m_queue_data.lock = 0;
    return wfOk;
}

WF_Queue m_wf_queue = {
    WF_Queue_Clear,
    WF_Queue_Append,
    WF_Queue_Read,
    WF_Queue_ReadAll};

////////////////////////////////////////
static void WF_MQTT_Queue_Clear(void);
static void WF_MQTT_Queue_Append(char *data, int len);
static WF WF_MQTT_Queue_Read(char *aim, int len, int timeout);
static WF WF_MQTT_Queue_ReadAll(char *aim, int *len, int timeout);

WF_QueueData m_MQTT_queue_data;

static void WF_MQTT_Queue_Clear(void)
{
    while (m_MQTT_queue_data.lock == 1)
    {
        osDelay(1);
    }
    m_MQTT_queue_data.lock = 1;

    m_MQTT_queue_data.cursor = m_MQTT_queue_data.buf;
    m_MQTT_queue_data.read = m_MQTT_queue_data.buf;

    m_MQTT_queue_data.len = 0;

    m_MQTT_queue_data.lock = 0;
}

static void WF_MQTT_Queue_Append(char *data, int len)
{
    while (m_MQTT_queue_data.lock == 1)
    {
        osDelay(1);
    }
    m_MQTT_queue_data.lock = 1;

    memcpy(m_MQTT_queue_data.cursor, data, len);
    m_MQTT_queue_data.cursor += len;
    m_MQTT_queue_data.len += len;

    m_MQTT_queue_data.lock = 0;
}

static WF WF_MQTT_Queue_Read(char *aim, int len, int timeout)
{
    int count = 0;
    while (m_MQTT_queue_data.lock == 1)
    {
        osDelay(1);
    }
    m_MQTT_queue_data.lock = 1;

    while (len > m_MQTT_queue_data.len)
    {
        m_MQTT_queue_data.lock = 0;
        osDelay(1);
        if (++count >= timeout)
        {
            //time out
            m_MQTT_queue_data.lock = 0;
            return wfTimeOut;
        }
    }

    memcpy(aim, m_MQTT_queue_data.read, len);

    m_MQTT_queue_data.read += len;
    m_MQTT_queue_data.len -= len;

    m_MQTT_queue_data.lock = 0;
    return wfOk;
}

static WF WF_MQTT_Queue_ReadAll(char *aim, int *len, int timeout)
{
    int count = 0;
    while (m_MQTT_queue_data.lock == 1)
    {
        osDelay(1);
    }
    m_MQTT_queue_data.lock = 1;

    while (m_MQTT_queue_data.len == 0)
    {
        m_MQTT_queue_data.lock = 0;
        osDelay(1);
        if (++count >= timeout)
        {
            //time out
            m_MQTT_queue_data.lock = 0;
            return wfTimeOut;
        }
    }

    *len = m_MQTT_queue_data.len;
    memcpy(aim, m_MQTT_queue_data.read, m_MQTT_queue_data.len);

    m_MQTT_queue_data.cursor = m_MQTT_queue_data.buf;
    m_MQTT_queue_data.read = m_MQTT_queue_data.buf;
    m_MQTT_queue_data.len = 0;

    m_MQTT_queue_data.lock = 0;
    return wfOk;
}

WF_Queue m_wf_MQTT_queue = {
    WF_MQTT_Queue_Clear,
    WF_MQTT_Queue_Append,
    WF_MQTT_Queue_Read,
    WF_MQTT_Queue_ReadAll};
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
    /*queue 初始化*/
    m_queue_data.buf = (char *)MALLOC(sizeof(char) * rbufSize);
    m_queue_data.cursor = m_queue_data.buf;
    m_queue_data.read = m_queue_data.buf;
    m_queue_data.len = 0;
    m_queue_data.lock = 0;

    m_MQTT_queue_data.buf = (char *)MALLOC(sizeof(char) * rbufSize);
    m_MQTT_queue_data.cursor = m_MQTT_queue_data.buf;
    m_MQTT_queue_data.read = m_MQTT_queue_data.buf;
    m_MQTT_queue_data.len = 0;
    m_MQTT_queue_data.lock = 0;

    wifi->pqueue = &m_wf_queue;
    wifi->mqttqueue = &m_wf_MQTT_queue;
    // link to local
    pwifi = wifi;
    return wifi;
}

void slot_link_hasData(void *data)
{
    char *str = (char *)data;
    pwifi->pqueue->append(str, pwifi->link->rCnt);
    // em_printf("\r\nwifi have data %d.\r\n", pwifi->link->rCnt);
    em_printf("ISR-->::%s\r\n------------------\r\n", str);
    if (strncmp(str, "\r\n+IPD", 6) == 0)
    {
        int m_sock, m_len; //获取到网络信息, 解析
        sscanf(str + 7, "%d,%d", &m_sock, &m_len);
        //em_printf("sock:%d, len:%d", m_sock, m_len);

        if (m_sock == pwifi->sock_mqtt)
        {
            //是mqtt的信息
            //em_printf("获取到mqtt的数据\n");
            while (*str != ':')
                str++;
            str++;
            pwifi->mqtt_read = 1;

            pwifi->mqttqueue->append(str, m_len);

            // for (int i = 0; i < m_len; i++)
            //     em_printf("%c-", str[i]);
            // em_printf("\n");
        }
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

static int contain(char *data, int len, char *str)
{
    int aimlen = strlen(str);
    if (len < aimlen)
        return -1;

    for (int i = 0; i <= len - aimlen; i++)
    {
        int flag = 1;
        for (int j = 0; j < aimlen; j++)
        {
            if (data[i + j] != str[j])
            {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            return 0;
        }
    }
    return -1;
}

static WF get_response(char *checkok, char *checkerror, int timeout)
{
    char str[512];
    int len;
    //set state
    pwifi->pqueue->clear();
    for (int i = 0; i < timeout / 20; i++)
    {
        len = 0;
        pwifi->pqueue->readAll(str, &len, 20);
        if (contain(str, len, checkok) == 0)
        {
            return wfOk;
        }
        if (contain(str, len, checkerror) == 0)
        {
            return wfError;
        }
    }
    return wfTimeOut;
}

static WF get_Net_response(char *checkok, char *checkerror, int timeout)
{
    char str[550];
    int len;
    //set state
    pwifi->pqueue->clear();
    em_printf(">in net response.\r\n");
    for (int i = 0; i < timeout / 20; i++)
    {
        len = 0;
        pwifi->pqueue->readAll(str, &len, 20);
        if (len > strlen(checkok))
        {
            // em_printf(">maybe stick package.\r\n");
            // em_printf(">str :: %s.\r\n", str);
            if (contain(str, len, checkok) == 0)
            {
                pwifi->pqueue->append(str + strlen(checkok), len - strlen(checkok));

                // 判断是否是mqtt的数据
                char *pstr = str + strlen(checkok) + 2;
                // em_printf(">%s.\r\n", pstr);
                if (strncmp(pstr, "+IPD", 4) == 0)
                {
                    int m_sock, m_len; //获取到网络信息, 解析
                    sscanf(pstr + 5, "%d,%d", &m_sock, &m_len);
                    // em_printf("mqtt>\r\n");
                    // em_printf("sock:%d, len:%d", m_sock, m_len);

                    if (m_sock == pwifi->sock_mqtt)
                    {
                        //是mqtt的信息
                        //em_printf("获取到mqtt的数据\n");
                        while (*pstr != ':')
                            pstr++;
                        pstr++;
                        pwifi->mqtt_read = 1;

                        pwifi->mqttqueue->append(pstr, m_len);

                        // for (int i = 0; i < m_len; i++)
                        //     em_printf("%c-", str[i]);
                        // em_printf("\n");
                    }
                }
                return wfOk;
            }
        }
        if (contain(str, len, checkok) == 0)
        {
            return wfOk;
        }
        if (contain(str, len, checkerror) == 0)
        {
            return wfError;
        }
    }
    return wfTimeOut;
}

inline static WF command_at_once(char *cmd, char *checkok, char *checkerror, int timeout)
{
    char str[512];
    int len;
    //set state
    pwifi->pqueue->clear();

    // send cmd
    em_printf("once> atoncef.\r\n");
    Serial_Print(pwifi->link, "%s\r\n", cmd);
    em_printf("once> atonces.\r\n");
    for (int i = 0; i < timeout / 20; i++)
    {
        len = 0;
        pwifi->pqueue->readAll(str, &len, 20);

        if (contain(str, len, checkok) == 0)
        {
            em_printf("once> ok.\r\n");
            return wfOk;
        }
        if (contain(str, len, checkerror) == 0)
        {
            return wfError;
            em_printf("once> error.\r\n");
        }
    }
    em_printf("once> timeout.\r\n");
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
    return command_at_once("AT", "OK\r\n", "ERROR\r\n", 100);
}
WF WIFI_AT_RST()
{
    //AT 测试
    // timeout:100 ms
    // at once -> OK
    if (command_at_once("AT+RST", "OK\r\n", "ERROR\r\n", 100) != wfOk)
    {
        // error
        return wfError;
    }
    //wait to restart
    return get_response("ready\r\n", "ERROR\r\n", 2000);
}
WF WIFI_AT_ATE()
{
    //关闭回显
    // timeout:100 ms
    return command_at_once("ATE0", "OK\r\n", "ERROR\r\n", 100);
}
WF WIFI_AT_SET_MODE(int mode)
{
    char str[32];
    sprintf(str, "AT+CWMODE_DEF=%d", mode);
    em_printf("--> %s\r\n", str);

    return command_at_once(str, "OK\r\n", "ERROR\r\n", 100);
}
WF WIFI_AT_SET_MUX()
{
    return command_at_once("AT+CIPMUX=1", "OK\r\n", "ERROR\r\n", 100);
}
WF WIFI_AT_SET_CWAUTOCONN(int mode)
{
    char str[32];
    sprintf(str, "AT+CWAUTOCONN=%d", mode);
    return command_at_once(str, "OK\r\n", "ERROR\r\n", 100);
}
WF WIFI_AT_SET_SMART(int mode)
{
    if (mode == 0)
    {
        return command_at_once("AT+CWSTOPSMART", "OK\r\n", "ERROR\r\n", 100);
    }
    else
    {
        return command_at_once("AT+CWSTARTSMART", "OK\r\n", "ERROR\r\n", 100);
    }
}
WF WIFI_AT_TCP(int sock, char *ip, int port)
{
    char str[128];
    sprintf(str, "AT+CIPSTART=%d,\"TCP\",\"%s\",%d", sock, ip, port);
    return command_at_once(str, "OK\r\n", "ERROR\r\n", 500);
}
WF WIFI_AT_TCP_SEND(int sock, int len)
{
    char str[32];
    em_printf("AT> TCP SEND.\r\n");
    sprintf(str, "AT+CIPSEND=%d,%d", sock, len);
    return command_at_once(str, ">", "ERROR\r\n", 100);
}
// WF WIFI_AT_IPSTATUS()
// {
//     // 查询网络连接信息
//     // timeout:100 ms
//     return command_at_func("AT+CIPSTATUS", "OK\r\n", cb_ipstatus, 100);
// }
// WF WIFI_AT_APINFO()
// {
//     // 查询AP信息
//     // timeout:100 ms
//     return command_at_func("AT+CWJAP?", "OK\r\n", cb_apinfo, 100);
// }
// WF WIFI_AT_IPINFO()
// {
//     // 查询IP信息
//     // timeout:100 ms
//     return command_at_func("AT+CIFSR", "OK\r\n", cb_ipinfo, 100);
// }

// WF WIFI_AT_PING()
// {
//     // Ping 测试
//     // timeout:10000 ms
//     return command_at_func("AT+PING=\"www.baidu.com\"", "OK\r\n", cb_ping, 10000);
// }

// WF WIFI_AT_TTT()
// {
//     //关闭上电自动连接
//     // timeout:100 ms
//     return command_at_once("AT+CWAUTOCONN=0", "OK\r\n", 100);
// }

/**/
WF WifiJoinAP(char *ssid, char *pwd)
{
    //连接AP
    // timeout:20000 ms
    char str[64];
    sprintf(str, "AT+CWJAP_CUR=\"%s\",\"%s\"", ssid, pwd);

    return command_at_once(str, "OK\r\n", "FAIL\r\n", 20000);
}

WF WifiSmart()
{
    em_printf(">>>start smart config.\r\n");
    if (WIFI_AT_SET_CWAUTOCONN(1) != wfOk)
    {
        em_printf("Set autoconnect ERROR\r\n");
        return wfError;
    }

    em_printf(">start smart.\r\n");
    if (WIFI_AT_SET_SMART(1) != wfOk)
    {
        em_printf("Start Smart ERROR.\r\n");
        return wfError;
    }

    while (1)
    {
        if (get_response("smartconfig connected wifi\r\n", "ERROR\r\n", 100) == wfOk)
        {
            break;
        }
    }
    em_printf("Smart Connect Success.\r\n");

    em_printf(">Stop smart.\r\n");
    if (WIFI_AT_SET_SMART(0) != wfOk)
    {
        em_printf("Stop Smart ERROR.\r\n");
        return wfError;
    }

    em_printf(">Close autoconnect.\r\n");
    if (WIFI_AT_SET_CWAUTOCONN(0) != wfOk)
    {
        em_printf("Set autoconnect ERROR\r\n");
        return wfError;
    }

    return wfOk;
}

WF WifiInit()
{
    em_printf(">>> wifi init start...\r\n");
    em_printf(">AT test...\r\n");
    if (WIFI_AT_CMD() != wfOk)
    {
        em_printf("at test ERROR.\r\n");
        return wfError;
    }
    em_printf(">Reset test...\r\n");
    if (WIFI_AT_RST() != wfOk)
    {
        em_printf("Reset test ERROR.\r\n");
        return wfError;
    }
    em_printf(">set no echo.\r\n");
    if (WIFI_AT_ATE() != wfOk)
    {
        em_printf("Set no echo ERROR.\r\n");
        return wfError;
    }
    em_printf(">Set wifi mode.[STATION]\r\n");
    if (WIFI_AT_SET_MODE(1) != wfOk)
    {
        em_printf("Set wifi mode ERROR.\r\n");
        return wfError;
    }
    em_printf(">Set Connect MUX.\r\n");
    if (WIFI_AT_SET_MUX() != wfOk)
    {
        em_printf("Set MUX ERROR.\r\n");
        return wfError;
    }
    em_printf(">Connect to AP.\r\n");
    if (WifiJoinAP("emeili", "88888888") != wfOk)
    {
        em_printf("Connect to AR ERROR.\r\n");

        return WifiSmart();
    }

    em_printf(">>> wifi init success.\r\n");
    return wfOk;
}

// void WifiSelfCheck()
// {
//     if (WIFI_AT_CMD() == wfOk) //AT test
//     {
//         pwifi->state = NoWifi;
//         //should reset<-v->
//         WIFI_AT_ATE();
//         //WIFI_AT_TTT();
//         //ap
//         if (WIFI_AT_IPSTATUS() == wfOk)
//         {
//             if (pwifi->ipstatus == 5)
//             {
//                 //NO AP
//             }
//             else
//             {
//                 //AP
//                 if (WIFI_AT_APINFO() == wfOk)
//                 {
//                     em_printf("root>>> AP: %s\n", pwifi->AP);
//                 }

//                 //IP
//                 if (WIFI_AT_IPINFO() == wfOk)
//                 {
//                     em_printf("root>>>  IP: %s\n", pwifi->IP);
//                     em_printf("root>>> MAC: %s\n", pwifi->MAC);
//                 }
//                 //PING

//                 if (WIFI_AT_PING() == wfOk)
//                 {
//                     em_printf("root>>>Ping: %d\n", pwifi->ping);
//                 }
//                 else
//                 {
//                     em_printf("no internet.\n");
//                 }

//                 if (pwifi->ipstatus == 3)
//                 {
//                     //连接情况
//                 }
//             }
//         }
//     }
// }

void WifiShowStatus()
{
    //wifi mode

    //ap

    // ip mac

    // ping
}
/*##############################################*/

socket getsocket()
{
    socket sock = -1;
    for (int i = 0; i < 5; i++)
    {
        if (pwifi->pool[i] == 0)
        {
            sock = i;
            return sock;
        }
    }
    return -1;
}

/*##############################################*/
socket TcpSocket(char *ipaddr, int port)
{
    char str[128];
    socket sock = getsocket(); //获取一个socket<一共有five> -- sockct pool

    if (sock == -1)
        return -1;

    if (WIFI_AT_TCP(sock, ipaddr, port) == wfOk)
    {
        pwifi->pool[sock] = 1;
        return sock;
    }
    return -1;
}

// // 255 广播地址 <xxx.xxx.xxx.255>
// socket UdpSocket(char *ipaddr, int port)
// {
//     char str[64];
//     socket sock = 1; //获取一个socket<一共有five> -- sockct pool
//     sprintf(str, "AT+CIPSTART=%d,\"UDP\",\"%s\",%d", sock, ipaddr, port);
//     if (command_at_once(str, "OK\r\n", 500) == wfOk)
//     {
//         //占用这个socket,并返回
//         return sock;
//     }
//     return -1;
// }

//data需要转化为字符串 +'\0'
//max(len) -- 2048
WF TcpSend(socket sock, char *data, int len)
{
    // check socket
    em_printf("tcp> begin.\r\n");
    if (WIFI_AT_TCP_SEND(sock, len) != wfOk)
    {
        em_printf("tcp> error.\r\n");
        return wfError;
    }
    em_printf("tcp> send.\r\n");
    NetSend(data, len);

    return get_Net_response("\r\nSEND OK\r\n", "ERROR\r\n", 300);
}
