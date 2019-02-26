/***************************************************************************************

                                    AT-Frame-Work
                     an universal AT instruction driver frame work

***************************************************************************************/
#ifndef _ATFW_H_
#define _ATFW_H_

#include <stdint.h>

typedef enum {
    ATFW_OK = 0,
    ATFW_TIMEOUT,
    ATFW_ERR,
} atfwres_t;

atfwres_t atfw_init(void);
atfwres_t atfw_sendwaitstr(const char *send, const char *expect, char *found, uint32_t foundsz, uint32_t timeout);
atfwres_t atfw_sendwaitbuf(const uint8_t *send, uint32_t sendsz, uint8_t *recv, uint32_t recvsz,
                           uint32_t *respsz, uint32_t timeout);
atfwres_t atfw_send(const uint8_t *send, uint32_t sendsz);
atfwres_t atfw_recv(uint8_t *recv, uint32_t recvsz, uint32_t *respsz, uint32_t timeout);
atfwres_t atfw_ioctl(void *res, char *fmt, ...);

#endif

