/***************************************************************************************

                                    AT-Frame-Work
                     an universal AT instruction driver frame work

***************************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "atfw.h"
#include "atfw_config.h"

typedef enum {
    STA_IDLE,
    STA_BUSY,
} sta_t;

static atfwres_t init_flag = ATFW_ERR;
static sta_t sta_flag;

/**
 * lock the atfw
 * @return 0 --- lock successfully
 *         1 --- lock failed
 */
#define LOCK()      ((sta_flag == STA_IDLE) ? (sta_flag = STA_BUSY, 0) : (1))
#define UNLOCK()    sta_flag = STA_IDLE;

/**
 * atfw initialization
 * @return: ATFW_OK if everything ok, ATFW_ERR if error happens
 */
atfwres_t atfw_init(void)
{
    if (LOCK()) {
        return ATFW_ERR;
    }

    if (at_init() != 0|| os_queue_init() != 0) {
        init_flag = ATFW_ERR;
        dbg_printf("err, the atfw initialization failed!\n");
        goto out;
    }
    init_flag = ATFW_OK;

out:
    UNLOCK();
    return init_flag;
}

/**
 * atfw send string to the hardware-at-device then wait for response(specified by user) from the latter
 * @param send: the string to be sent
 * @param expect: the string you expect to receive(note that if the receive string contains the @expect
 *                this function will consider it gets the @expect), NULL indicates you don't care what
 *                you get, all received string will be copied to @found if possible
 * @param found: if the string specified by @expect received, the matched part of receive string will be
 *               copied to @found. if you don't want it copy, let it be NULL
 * @param foundsz: the size(in bytes) of @found. if you don't want it copy, let it be NULL
 * @param timeout: the timeout(in ms) while receiving the response you expected
 * @return: ATFW_OK if everything ok, ATFW_ERR if error happens
 */
atfwres_t atfw_sendwaitstr(const char *send, const char *expect, char *found, uint32_t foundsz, uint32_t timeout)
{
    static char resp[CFG_RECV_BUF_SIZE];
    uint32_t respsz;
    char *p;
    atfwres_t res;

    /*
     * check parameters
     */
    if (init_flag != ATFW_OK) {
        res = ATFW_ERR;
        goto err;
    }

    if (!send || !strlen(send)) {
        res = ATFW_ERR;
        goto err;
    }

    if (os_getrunenv() != 1) {
        res = ATFW_ERR;
        goto err;
    }

    if (LOCK()) {
        return ATFW_ERR;
    }

    /*
     * now, the real process begin
     */
    if (at_send(send, strlen(send)) != ATFW_OK) {
        res = ATFW_ERR;
        goto err;
    }

    if (!os_queue_wait(resp, sizeof(resp), &respsz, timeout)) {        // if we got the response
        if (respsz > sizeof(resp) - 1) {
            dbg_printf("warn, the buffer(len=%d) matched in the response is larger than @found(len=%d),"
                       " which means you may lost some information", respsz, foundsz);
            resp[sizeof(resp)] = 0;
        } else {
            resp[respsz] = 0;
        }

        if (expect) {
            // TODO  is it better to implement in regular-expression?
            // if the response from hardware-at-device is what we expected
            if (p = strstr(resp, expect)) {
                if (found && foundsz) {
                    if (respsz <= foundsz - 1) {
                        strncpy(found, resp, respsz);
                        found[respsz] = 0;
                    } else {
                        dbg_printf("warn, the buffer(len=%d) matched in the response is larger than @found(len=%d),"
                                   " which means you may lost some information", respsz, foundsz);
                    }
                }
            } else {
                res = ATFW_ERR;
                goto err;
            }
        } else {  // if @expect is NULL, which means we don't care what we received, just copy it to @found
            if (found && foundsz) {
                if (respsz <= foundsz - 1) {
                    strncpy(found, resp, respsz);
                    found[respsz] = 0;
                } else {
                    dbg_printf("warn, the buffer(len=%d) matched in the response is larger than @found(len=%d),"
                               " which means you may lost some information", respsz, foundsz);
                }
            }
        }
    } else {
        res = ATFW_TIMEOUT;
        goto err;
    }

    UNLOCK();
    return ATFW_OK;

err:
    UNLOCK();
    return res;
}

/**
 * atfw send buffer(any data) to the hardware-at-device then wait for any response from the latter
 * @param send: the buffer to be sent
 * @param sendsz: the size(in bytes) of @send
 * @param recv: the buffer will the received data copy to
 * @param recvsz: the size(in bytes) of @recv
 * @param respsz: the size(in bytes) of hardware-at-device received data
 * @param timeout: the timeout(in ms) while receiving the response
 * @return: ATFW_OK if everything ok, ATFW_ERR if error happens
 */
atfwres_t atfw_sendwaitbuf(const uint8_t *send, uint32_t sendsz, uint8_t *recv, uint32_t recvsz,
                           uint32_t *respsz, uint32_t timeout)
{
    static char resp[CFG_RECV_BUF_SIZE];
    atfwres_t res;

    /*
     * check parameters
     */
    if (init_flag != ATFW_OK) {
        res = ATFW_ERR;
        goto err;
    }

    if (!send || !sendsz) {
        res = ATFW_ERR;
        goto err;
    }

    if (os_getrunenv() != 1) {
        res = ATFW_ERR;
        goto err;
    }

    if (LOCK()) {
        return ATFW_ERR;
    }

    /*
     * now, the real process begin
     */
    if (at_send(send, sendsz) != ATFW_OK) {
        res = ATFW_ERR;
        goto err;
    }

    if (!os_queue_wait(resp, sizeof(resp), respsz, timeout)) {        // if we got the response
        if (recv && recvsz) {
            if (*respsz <= recvsz) {
                memcpy(recv, resp, *respsz);
            } else {
                memcpy(recv, resp, recvsz);
                dbg_printf("warn, the response-buffer(len=%d) is larger than @recv(len=%d),"
                           " which means you may lost some information", *respsz, recvsz);
            }
        }
    } else {
        res = ATFW_TIMEOUT;
        goto err;
    }

    UNLOCK();
    return ATFW_OK;

err:
    UNLOCK();
    return res;
}

/**
 * just send buffer to hardware-at-device, without receiving
 * @param send: the data to be sent
 * @param sendsz: the size(in bytes) of @send
 * @return: ATFW_OK if everything ok, ATFW_ERR if error happens
 */
atfwres_t atfw_send(const uint8_t *send, uint32_t sendsz)
{
    if (init_flag != ATFW_OK) {
        goto err;
    }

    if (!send && !sendsz) {
        goto err;
    }

    if (LOCK()) {
        return ATFW_ERR;
    }

    if (at_send(send, sendsz)) {
        goto err;
    }

    UNLOCK();
    return ATFW_OK;

err:
    UNLOCK();
    return ATFW_ERR;
}

/**
 * receive data from hardware-at-device
 * @param recv: the buffer will the received data copy to
 * @param recvsz: the size(in bytes) of @recv
 * @param respsz: the size(in bytes) of hardware-at-device received data
 * @param timeout: the timeout(in ms) while receiving data
 * @return: ATFW_OK if everything ok, ATFW_ERR if error happens
 */
atfwres_t atfw_recv(uint8_t *recv, uint32_t recvsz, uint32_t *respsz, uint32_t timeout)
{
    static char resp[CFG_RECV_BUF_SIZE];

    if (init_flag != ATFW_OK) {
        goto err;
    }

    if (!recv || !recvsz || !respsz) {
        goto err;
    }

    if (os_getrunenv() != 1) {
        goto err;
    }

    if (LOCK()) {
        return ATFW_ERR;
    }

    if (!os_queue_wait(resp, sizeof(resp), respsz, timeout)) {        // if we got the response
        if (*respsz <= recvsz) {
            memcpy(recv, resp, *respsz);
        } else {
            memcpy(recv, resp, recvsz);
            dbg_printf("warn, the response-buffer(len=%d) is larger than @recv(len=%d),"
                       " which means you may lost some information", *respsz, recvsz);
        }
    } else {
        goto err;
    }

    UNLOCK();
    return ATFW_OK;

err:
    UNLOCK();
    return ATFW_ERR;
}

/**
 * io-control of hardware-at-device
 * @param cmd: the user command to control hardware-at-device
 * @return: ATFW_OK if everything ok, ATFW_ERR if error happens
 */
atfwres_t atfw_ioctl(void *cmd)
{
    if (init_flag != ATFW_OK) {
        return ATFW_ERR;
    }

    if (os_getrunenv() != 1) {
        return ATFW_ERR;
    }

    if (!cmd) {
        return ATFW_ERR;
    }

    if (LOCK()) {
        return ATFW_ERR;
    }

    if (at_ioctl(cmd)) {
        UNLOCK();
        return ATFW_ERR;
    }

    UNLOCK();
    return ATFW_OK;
}

