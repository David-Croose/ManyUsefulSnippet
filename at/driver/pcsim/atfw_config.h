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
#include <unistd.h>
#include <stdint.h>
#include <string.h>

static uint8_t sendbuf[100];
static uint8_t recvbuf[100];
static uint32_t sendlen;

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
    while (timeout--) {
        if (sendlen) {
            sendlen = 0;

            if (!(strcmp((const char *)sendbuf, "AT: init\n"))) {
                strcpy(buf, "OK\n");
                *recvsz = strlen("OK\n");
            } else if (!(strcmp((const char *)sendbuf, "AT: open\n"))) {
                strcpy(buf, "OK\n");
                *recvsz = strlen("OK\n");
            } else if (!(strcmp((const char *)sendbuf, "AT: close\n"))) {
                strcpy(buf, "OK\n");
                *recvsz = strlen("OK\n");
            } else if (!(strcmp((const char *)sendbuf, "AT: send\n"))) {
                strcpy(buf, "OK, waiting for input\n");
                *recvsz = strlen("OK, waiting for input\n");
            } else if (!(strcmp((const char *)sendbuf, "AT: status\n"))) {
                strcpy(buf, "OK\nsignal: good, voltage: good, waiting for commands\n");
                *recvsz = strlen("OK\nsignal: good, voltage: good, waiting for commands\n");
            } else if (sendbuf[0] == 0x11 && sendbuf[1] == 0x22 && sendbuf[2] == 0x33
                       && sendbuf[3] == 0x44 && sendbuf[4] == 0x55) {
                buf[0] = 0xA0;
                buf[1] = 0xA1;
                buf[2] = 0xA2;
                buf[3] = 0xA3;
                *recvsz = 4;
            } else if (sendbuf[0] == 0xAA && sendbuf[1] == 0xBB && sendbuf[2] == 0xCC
                       && sendbuf[3] == 0xDD && sendbuf[4] == 0xEE && sendbuf[5] == 0xFF) {
                buf[0] = 0xB0;
                buf[1] = 0xB1;
                buf[2] = 0xB2;
                buf[3] = 0xB3;
                buf[4] = 0xB4;
                *recvsz = 5;
            } else {
                return 2;
            }

            return 0;
        }
        usleep(1000);
    }

    return 1;
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
    return 0;
}

static int at_send(const uint8_t *send, uint32_t sendsz)
{
    memset(sendbuf, 0, sizeof(sendbuf));
    if (sendsz <= sizeof(sendbuf)) {
        memcpy(sendbuf, send, sendsz);
        sendlen = sendsz;
        return 0;
    } else {
        return 1;
    }
}

static int at_ioctl(void *cmd)
{
    return 0;
}

#endif

