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

int os_queue_send(const uint8_t *resp, uint32_t respsz)
{
    return 0;
}

/***************************************************************************************

                                 The hardware portion

***************************************************************************************/
static int at_init(void)
{
    at_send();


    return 0;
}

static int at_send(const uint8_t *send, uint32_t sendsz)
{

}

typedef enum {
    SIM7X00_RES_OK,
    SIM7X00_RES_SOCK_ACTIVE,
    SIM7X00_RES_SOCK_DEACTIVE,
    SIM7X00_RES_TIMEOUT,
    SIM7X00_RES_RESP_ERR,
    SIM7X00_RES_NOT_SUPPORT_CMD,
} sim7x00_res_t;

static sim7x00_res_t _sendwait(const char *send, const char *expect, char *recv, uint32_t timeout)
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

static int at_ioctl(void *cmd)
{
    char recv[100] = {0};
    sim7x00_res_t res;
    char *p;
    int32_t rssi, ber;

    if (!(strcmp((const char *)cmd, "set transparency mode"))) {

    } else if (!(strcmp((const char *)cmd, "set polling mode"))) {

    } else if (!(strcmp((const char *)cmd, "get socket status"))) {
        if ((res = _sendwait("AT+NETOPEN?\n", 0, recv, 100)) == SIM7X00_RES_OK)  {
            if (p = strstr(recv, "+NETOPEN:")) {
                if (p[strlen("+NETOPEN:")] == '1') {
                    return SIM7X00_RES_SOCK_ACTIVE;
                } else {
                    return SIM7X00_RES_SOCK_DEACTIVE;
                }
            }
        } else {
            return res;
        }
    } else if (!strcmp((const char *)cmd, "get signal strength")) {
        if ((res = _sendwait("AT+CSQ\n", "+CSQ: ", recv, 100)) == SIM7X00_RES_OK) {
            rssi = strtol(&recv[strlen("+CSQ: ")], &p, 10); 
            ber = strtol(++p, 0, 10); 
        } else {
            return res;
        }
    } else if (!strcmp((const char *)cmd, "hard reset")) {

    } else if (!strcmp((const char *)cmd, "soft reset")) {
        return _sendwait("AT+CRESET\n", "OK", 0, 100);
    } else if (!strcmp((const char *)cmd, "set cmd echo enable")) {
        return _sendwait("ATE1\n", "OK", 0, 100);
    } else if (!strcmp((const char *)cmd, "set cmd echo disable")) {
        return _sendwait("ATE0\n", "OK", 0, 100);
    } else if (!strcmp((const char *)cmd, "get dns ip")) {

    } else if (!strcmp((const char *)cmd, "get local ip")) {

    } else if (!strcmp((const char *)cmd, "get remote ip")) {

    } else if (!strcmp((const char *)cmd, "set baud 115200")) {

    } else if (!strcmp((const char *)cmd, "set baud 921600")) {

    }

    return SIM7X00_RES_NOT_SUPPORT_CMD;
}

#endif

