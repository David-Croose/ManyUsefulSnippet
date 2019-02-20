/***************************************************************************************

                                    AT-Frame-Work
                     an universal AT instruction driver frame work

***************************************************************************************/
#include <stdint.h>
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

    if (at_init() < 0 || os_queue_init() < 0) {
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
 * atfw send data(@send) to the remote then wait response from the remote
 * @param send: the data to be sent
 * @param sendsz: the length(in bytes) of @data
 * @param recv: the data(string) you expect to receive(note that if the receive data contains the
 *              @recv, this function will consider it gets the @recv)
 * @param found: if the @recv received, @recv will be copied to @found
 * @param foundsz: the size(in bytes) of @found
 * @param timeout: the timeout(in ms) while receiving the data you expected
 * @return: ATFW_OK if everything ok, ATFW_ERR if error happens
 */
// TODO  maybe you should add a parameter: recvsz, indicates how many bytes it gets
atfwres_t atfw_sendwait(const uint8_t *send, uint32_t sendsz, const char *recv, 
                        uint8_t *found, uint32_t foundsz, uint32_t timeout)
{
    static uint8_t resp[CFG_RECV_BUF_SIZE];
    uint32_t respsz;
    uint8_t *p;

    if (LOCK()) {
        return ATFW_ERR;
    }

    /*
     * check parameters
     */
    if (init_flag != ATFW_OK) {
        dbg_printf("err, the atfw has not initialized yet!\n");
        goto err;
    }

    if (os_getrunenv() != ENV_RTOS) {
        dbg_printf("err, @%s can only run in RTOS environment!\n", __FUNCTION__);
        goto err;
    }

    if (!send && !sendsz) {
        dbg_printf("err, @send and @sendsz error in @%s\n", __FUNCTION__);
        goto err;
    }

    /* 
     * now, the real process begin
     */
    if (at_send(send, sendsz) != ATFW_OK) {
        dbg_printf("err, at-device send failed\n");
        goto err;
    }

    memset(resp, 0, sizeof(resp));
    if (!os_queue_wait(resp, sizeof(resp), &respsz, timeout)) {        // if we got the response
        if (recv) {
            // TODO  is it better to implement in regular-expression?
            if (p = strstr(resp, recv)) {  // it is accepted that if the response contains what we expected
                if (found && foundsz) {
                    memset(found, 0, foundsz);
                    strncpy(found, p, foundsz);
                    if (strlen(p) > foundsz) {
                        dbg_printf("warn, the buffer(len=%d) matched in the response is larger than @found(len=%d),"
                                   " which means you may lost some information", respsz, foundsz);
                    }
                }
            } else {
                goto err;
            }
        } else {  // if @recv is NULL, which means we don't care what we received, just copy it to @found
            if (found && foundsz) {
                memset(found, 0, foundsz);
                if (respsz <= foundsz) {
                    memcpy(found, p, respsz);
                } else {
                    memcpy(found, p, foundsz);
                    dbg_printf("warn, the buffer(len=%d) matched in the response is larger than @found(len=%d),"
                               " which means you may lost some information", respsz, foundsz);
                }
            }
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
 * just send data via at-device
 * @param send: the data to be sent
 * @param sendsz: the length(in bytes) of @data
 * @return: ATFW_OK if everything ok, ATFW_ERR if error happens
 */
atfwres_t atfw_send(const uint8_t *send, uint32_t sendsz)
{
    if (LOCK()) {
        return ATFW_ERR;
    }

    if (send && sendsz) {
        if (at_send(send, sendsz)) {
            goto err;
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
 * receive data from at-device
 * @param recv: the received data from hardware at-device
 * @param recvsz: the length(in bytes) of @recv
 * @note: if user application wants to receive data from at-device, use @atfw_sendwait.
 *        this function, however, runs in low-level driver, receives first-hand data
 *        from hardware at-device then sends it to @atfw_sendwait
 */
void atfw_recv(const uint8_t *recv, uint32_t recvsz)
{
    if (recv && recvsz) {
        os_queue_send(recv, recvsz);
    }
}

/**
 * io-control of hardware at-device
 * @param cmd: the user command to control hardware at-device
 * @return: ATFW_OK if everything ok, ATFW_ERR if error happens
 */
atfwres_t atfw_ioctl(void *cmd)
{
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

