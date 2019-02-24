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

#define SIM7X00_RES_SOCK_ACTIVE      5
#define SIM7X00_RES_SOCK_DEACTIVE      5
#define SIM7X00_RES_TIMEOUT         5
#define SIM7X00_RES_RESP_ERR         5
#define SIM7X00_RES_NOT_SUPPORT_CMD         5

static int _sendwait(const char *send, const char *expect, char **recv, uint32_t timeout)
{
    static char recvbuf[100];

    memset(recvbuf, 0, sizeof(recvbuf));
    at_send(send);
    if (!(os_queue_wait(recvbuf, sizeof(recvbuf), 0, timeout))) {
        if (*recv = strstr(recvbuf, expect)) {
            if (p[strlen("+NETOPEN:")] == '1') {
                return SIM7X00_RES_SOCK_ACTIVE;
            } else {
                return SIM7X00_RES_SOCK_DEACTIVE;
            }
        } else {
            return SIM7X00_RES_RESP_ERR;
        }
    } else {
        return SIM7X00_RES_TIMEOUT;
    }
}

static int at_ioctl(void *cmd)
{

    if (!(strcmp((const char *)cmd, "set transparency mode"))) {

    } else if (!(strcmp((const char *)cmd, "set polling mode"))) {

    } else if (!(strcmp((const char *)cmd, "get socket status"))) {
        at_send("AT+NETOPEN?\n");
        if (!(os_queue_wait(recvbuf, sizeof(recvbuf), 0, 100))) {
            if (p = strstr((const char *)recvbuf, "+NETOPEN:")) {
                if (p[strlen("+NETOPEN:")] == '1') {
                    return SIM7X00_RES_SOCK_ACTIVE;
                } else {
                    return SIM7X00_RES_SOCK_DEACTIVE;
                }
            } else {
                return SIM7X00_RES_RESP_ERR;
            }
        } else {
            return SIM7X00_RES_TIMEOUT;
        }
    } else if (!strcmp((const char *)cmd, "get signal strength")) {


    } else if (!strcmp((const char *)cmd, "hard reset")) {

    } else if (!strcmp((const char *)cmd, "soft reset")) {

    } else if (!strcmp((const char *)cmd, "set cmd echo enable")) {
        at_send("ATE1");
        if (!(os_queue_wait(recvbuf, sizeof(recvbuf), 0, 100))) {
            if (p = strstr((const char *)recvbuf, "+NETOPEN:")) {
                if (p[strlen("+NETOPEN:")] == '1') {
                    return SIM7X00_RES_SOCK_ACTIVE;
                } else {
                    return SIM7X00_RES_SOCK_DEACTIVE;
                }
            } else {
                return SIM7X00_RES_RESP_ERR;
            }
        } else {
            return SIM7X00_RES_TIMEOUT;
        }
    } else if (!strcmp((const char *)cmd, "set cmd echo disable")) {
        at_send("ATE0");
            
    } else if (!strcmp((const char *)cmd, "get dns ip")) {
            
    } else if (!strcmp((const char *)cmd, "get local ip")) {

    } else if (!strcmp((const char *)cmd, "get remote ip")) {
            
    }

    return SIM7X00_RES_NOT_SUPPORT_CMD;
}

#endif

