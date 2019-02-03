/***************************************************************************************

                                    AT-Frame-Work
                     an universal AT instruction driver frame work

***************************************************************************************/

#include <stdint.h>
#include <string.h>
#include "atfw.h"
#include "atfw_config.h"

static atfwres_t init_flag = ATFW_ERR;
static int sending_flag;
static int receiving_flag;

static atfwres_t findstr(const char *str, const char *pat, char *found, uint32_t foundsz)
{
    if (!str || !pat) {
        dbg_printf("err, the @str and @pat of @%s can not be NULL!\n", __FUNC__);
        return ATFW_ERR;
    }


}

atfwres_t atfw_init(void)
{
    if (at_init() < 0 || os_queue_init() < 0) {
        init_flag = ATFW_ERR;
        dbg_printf("err, the atfw initialization failed!\n");
        return init_flag;
    }
    init_flag = ATFW_OK;
    return init_flag;
}

/**
 * AT-Frame-Work send data(@send) to the remote then wait response from the remote
 * @param send: the data to be sent
 * @param aftsend: After-Send. is a callback routine performed after the @send has been sent
 * @param recv: the data you expect to receive(note that if the receive data contains the
 *              @recv, this function will consider it gets the @recv)
 * @param aftrecv: After-Receive. is a callback routine performed after the @recv has been
 *                 received
 * @param found: if the @recv received, @recv will be copy to @found
 * @param foundsz: the size(in bytes) of @found
 * @param timeout: the timeout(in ms) while receiving the data you expected
 * @return: ATFW_OK if everything ok, see @atfwres_t for more details
 */
atfwres_t atfw_sendwait(const char *send, cb_t *aftsend, const char *recv, cb_t *aftrecv,
                        char *found, uint32_t foundsz, uint32_t timeout)
{
    static char buf[CFG_RECV_BUF_SIZE];
    char *p;

    /*
     * check parameter
     */
    if (init_flag != ATFW_OK) {
        dbg_printf("err, the atfw has not initialized yet!\n");
        return ATFW_ERR;
    }

    if (os_getrunenv()) {
        dgb_printf("err, atfw can only run in RTOS environment!\n");
        return ATFW_ERR;
    }

    if (!send && !aftsend) {
        dgb_printf("err, @send and @aftsend of @%s can not be NULL"
                   "at the same time!\n", __FUNC__);
        return ATFW_ERR;
    }

    /* 
     * now, the real process begin
     */
    if (send) {
        if (at_send(send, strlen(send)) != ATFW_OK) {
            dgb_printf("err, at send failed!s\n");
            return ATFW_ERR;
        }
    }

    if (aftsend) {
        aftsend();
    }

    memset(buf, 0, sizeof(buf));
    if (!os_queue_wait(buf, sizeof(buf), timeout)) {      // if we got the queue
        if (p = strstr(buf, recv)) {
            if (found && foundsz) {
                memset(found, 0, foundsz);
                strncpy(found, p, foundsz);
            }
        } else if (!recv) {
            if (found && foundsz) {
                memset(found, 0, foundsz);
                strncpy(found, buf, foundsz);
            }
        } else {
            dgb_printf("err, the at cmd(%s) got a wrong response!\n", send);
            return ATFW_ERR;
        }
    } else {
        dgb_printf("err, the at cmd(%s) not responding!\n", send);
        return ATFW_ERR;
    }

    if (aftrecv) {
        aftrecv();
    }

    return ATFW_OK;
}

atfwres_t atfw_send(const char *send, uint32_t sendsz)
{
    if (send && sendsz) {
        at_send(send, sendsz);
    }
    return ATFW_OK;
}

/**
 *
 * 
 */
void atfw_recv(const char *recv, uint32_t recvsz)
{
    if (recv && recvsz) {
        os_queue_send(recv, recvsz);
    }
}

atfwres_t atfw_ioctl(void *cmd)
{

}

