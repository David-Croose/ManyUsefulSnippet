/***************************************************************************************

                                    AT-Frame-Work
                     an universal AT instruction driver frame work

***************************************************************************************/
#ifndef _ATFW_CONFIG_H_
#define _ATFW_CONFIG_H_

/***************************************************************************************

                                    The configuration

***************************************************************************************/
#define CFG_DEBUG_ENABLE        (1)
#define CFG_RECV_BUF_SIZE       (1600)

/***************************************************************************************

                                    The debug portion

***************************************************************************************/
#if (CFG_DEBUG_ENABLE == 1)
#include <stdio.h>
#define dbg_printf(fmt...)    printf(fmt)
#else
#define dbg_printf(x)    ...
#endif

/***************************************************************************************

                                    The RTOS portion

***************************************************************************************/
#include <stdint.h>
#include <string.h>

/**
 * get run environment
 * @return: 1 --- RTOS
 *          0 --- BareMetal
 */
static int os_getrunenv(void)
{
    return 1;
}

static int os_queue_init(void)
{
    return 0;
}

static int os_queue_wait(uint8_t *buf, uint32_t bufsz, uint32_t *recvsz, uint32_t timeout)
{

}

static void os_delay(uint32_t ms)
{

}

int os_queue_send(const uint8_t *resp, uint32_t respsz)
{
    return 0;
}

/***************************************************************************************

                                 The hardware portion

***************************************************************************************/
static sim7x00_resp_t at_init(void)
{
    sim7x00_resp_t resp;
    sim7x00_csq_t csq;
    char ip[50] = {0};

    if ((resp = at_ioctl(0, "set non-transparent mode")) != SIM7X00_RESP_OK) {
        return resp;
    }

    if ((resp = at_ioctl(&csq, "get signal strength")) != SIM7X00_RESP_OK) {
        return resp;
    }

    if ((resp = at_ioctl(0, "set cmd echo disable")) != SIM7X00_RESP_OK) {
        return resp;
    }

    if ((resp = at_ioctl(0, "open network")) != SIM7X00_RESP_OK) {
        return resp;
    }

    if ((resp = at_ioctl(ip, "get dns ip:route.cim120.cn")) != SIM7X00_RESP_OK) {
        return resp;
    }

    // TODO  there is more work to be done

    return SIM7X00_RESP_OK;
}

static int at_send(const uint8_t *send, uint32_t sendsz)
{

}

typedef enum {
    SIM7X00_RESP_OK,
    SIM7X00_RESP_TIMEOUT,
    SIM7X00_RESP_RESP_ERR,
    SIM7X00_RESP_NOT_SUPPORT_CMD,
} sim7x00_resp_t;

static sim7x00_resp_t _sendwait(const char *send, const char *expect, char *recv, uint32_t timeout)
{
    uint8_t recvbuf[100] = {0};
    char *p;

    at_send(send, strlen(send));
    if (!(os_queue_wait(recvbuf, sizeof(recvbuf), 0, timeout))) {
        if (exepect) {
            if (p = strstr(recvbuf, expect)) {
                if (recv) {
                    strcpy(recv, p);
                    recv[strlen(recv)] = 0;
                }
            } else {
                return SIM7X00_RES_RESP_ERR;
            }
        } else {
            if (recv) {
                strcpy(recv, recvbuf);
                recv[strlen(recv)] = 0;
            }
        }
    } else {
        return SIM7X00_RES_TIMEOUT;
    }

    return SIM7X00_RES_OK;
}

#define SIM7X00_TRANSPARENT_MODE        (1)
#define SIM7X00_NONTRANSPARENT_MODE     (0)
#define SIM7X00_SOCK_ACTIVE             (1)
#define SIM7X00_SOCK_DEACTIVE           (0)

typedef struct {
    int32_t rssi;
    int32_t ber;
} sim7x00_csq_t;

static sim7x00_resp_t at_ioctl(void *res, const char *cmd)
{
    char recv[100] = {0};
    char sendbuf[100] = {0};
    sim7x00_resp_t resp;
    char *p, *q;
    uint32_t i;
    char buf1[50];
    char buf2[50];

    if (!(strcmp(cmd, "set transparent mode"))) {
        return _sendwait("AT+CIPMODE=1\n", "OK", 0, 100);
    } else if (!(strcmp(cmd, "set non-transparent mode"))) {
        return _sendwait("AT+CIPMODE=0\n", "OK", 0, 100);
    } else if (!(strcmp(cmd, "get trans mode"))) {
        if ((resp = _sendwait("AT+CIPMODE?\n", "+CIPMODE: ", recv, 100)) == SIM7X00_RES_OK)  {
            if (recv[strlen("+CIPMODE: ")] == '1') {
                *(uint32_t *)res = SIM7X00_TRANSPARENT_MODE;
            } else {
                *(uint32_t *)res = SIM7X00_NONTRANSPARENT_MODE;
            }
        }
        return resp;
    } else if (!(strcmp(cmd, "switch to data mode"))) {
        return _sendwait("ATO\n", "CONNECT", 0, 100);
    } else if (!(strcmp(cmd, "switch to cmd mode"))) {
        _sendwait("+++\n", 0, 0, 100);
        os_delay(1100);
        return _sendwait("+++\n", "OK", 0, 100);
    } else if (!(strcmp(cmd, "get socket status"))) {
        if ((resp = _sendwait("AT+NETOPEN?\n", 0, recv, 100)) == SIM7X00_RES_OK)  {
            if (p = strstr(recv, "+NETOPEN: ")) {
                if (p[strlen("+NETOPEN: ")] == '1') {
                    *(uint32_t *)res = SIM7X00_SOCK_ACTIVE;
                } else {
                    *(uint32_t *)res = SIM7X00_SOCK_DEACTIVE;
                }
            }
        }
        return resp;
    } else if (!(strcmp(cmd, "open network"))) {
        if ((resp = _sendwait("AT+NETOPEN\n", "+NETOPEN: 0", recv, 100)) == SIM7X00_RES_OK) {
            if (!(strstr(recv, "OK"))) {
                return SIM7X00_RESP_RESP_ERR;
            }
        }
        return resp;
    } else if (!(strcmp(cmd, "close network"))) {
        if ((resp = _sendwait("AT+NETCLOSE\n", "+NETCLOSE: 0", recv, 100)) == SIM7X00_RES_OK) {
            if (!(strstr(recv, "OK"))) {
                return SIM7X00_RESP_RESP_ERR;
            }
        }
        return resp;
    } else if (p = strstr(cmd, "connect TCP server:")) {  // e.g. "connect TCP server:192.168.1.1,5678"
        for (p += strlen("connect TCP server:"), i = 0; isdigit(*p) && *p == '.'; p++, i++) {
            buf1[i] = *p;
        }
        buf1[i] = 0;

        for (p += 1, i = 0; isdigit(*p); p++, i++) {
            buf2[i] = *p;
        }
        buf2[i] = 0;

        memset(sendbuf, 0, sizeof(sendbuf));
        sprintf(sendbuf, "AT+CIPOPEN=0,\"TCP\",\"%s\",%s", buf1, buf2);
        if ((resp = _sendwait(sendbuf, 0, recv, 100)) == SIM7X00_RES_OK) {
            if (strstr(recv, "OK") && p = strstr(recv, "+CIPOPEN: 0,0")) {
                return SIM7X00_RESP_OK;
            }
        }
        return resp;
    } else if (!strcmp(cmd, "disconnect TCP server")) {
        return _sendwait("AT+CIPCLOSE=0\n", "OK", 0, 100);
    } else if (!strcmp(cmd, "get signal strength")) {
        if ((resp = _sendwait("AT+CSQ\n", "+CSQ: ", recv, 100)) == SIM7X00_RES_OK) {
            ((sim7x00_csq_t *)res)->rssi = strtol(&recv[strlen("+CSQ: ")], &p, 10);
            ((sim7x00_csq_t *)res)->ber = strtol(++p, 0, 10);
        }
        return resp;
    } else if (!strcmp(cmd, "hard reset")) {
        /* not support yet */
    } else if (!strcmp(cmd, "soft reset")) {
        return _sendwait("AT+CRESET\n", "OK", 0, 100);
    } else if (!strcmp(cmd, "set cmd echo enable")) {
        return _sendwait("ATE1\n", "OK", 0, 100);
    } else if (!strcmp(cmd, "set cmd echo disable")) {
        return _sendwait("ATE0\n", "OK", 0, 100);
    } else if (p = strstr(cmd, "get dns ip:")) {  // e.g. "get dns ip:www.baidu.com"
        memset(buf1, 0, sizeof(buf1));
        strcpy(buf1, &p[strlen("get dns ip:")]):
        memset(sendbuf, 0, sizeof(sendbuf));
        sprintf(sendbuf, "AT+CDNSGIP=\"%s\"\n", buf1);
        if ((resp = _sendwait(sendbuf, "+CDNSGIP: 1,", recv, 100)) == SIM7X00_RES_OK)  {
            if (p = strstr(recv, "\",\"")) {
                for (p += 3, q = (char *)res; isdigit(*p) || *p == '.'; p++, q++) {
                    *q = *p;
                }
            } else {
                return SIM7X00_RESP_RESP_ERR;
            }
        }
        return resp;
    } else if (!strcmp(cmd, "get local ip")) {
        if ((resp = _sendwait("AT+IPADDR\n", "+IPADDR: ", recv, 100)) == SIM7X00_RES_OK)  {
            if (strstr(recv, "OK")) {
                for (p = &recv[strlen("+IPADDR: ")], q = (char *)res; isdigit(*p) || *p == '.'; p++, q++) {
                    *q = *p;
                }
            } else {
                strcpy((char *)res, &recv[strlen("+IPADDR: ")]);
                return SIM7X00_RESP_RESP_ERR;
            }
        }
        return resp;
    } else if (p = strstr(cmd, "set baudrate:")) { // e.g. "set baudrate:921600"
        i = strtol(&p[strlen("set baudrate:")], 0, 10);
        memset(sendbuf, 0, sizeof(sendbuf));
        sprintf(sendbuf, "AT+IPR=%s\n", &p[strlen("set baudrate:")]);
        return _sendwait(sendbuf, "OK", 0, 100);
    }

    return SIM7X00_RES_NOT_SUPPORT_CMD;
}

#endif

