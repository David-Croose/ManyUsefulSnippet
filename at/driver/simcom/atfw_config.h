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
#include <stdarg.h>

static int dbg_printf(const char *fmt, ...)
{


}
#else
#define dbg_printf(x)    ...
#endif
}

/***************************************************************************************

                                    The RTOS portion

***************************************************************************************/
#include "FreeRTOS.h"

typedef enum {
    ENV_BAREMATAL,
    ENV_RTOS,
} env_t;

/**
 * get running environment
 */
static env_t os_getrunenv(void)
{


}

static int os_delay(uint32_t ms)
{
    return 0;
}

static int os_queue_init()
{

}

static int os_queue_wait()
{


}

static int os_queue_send()
{

}

/***************************************************************************************

                                 The hardware portion

***************************************************************************************/
#include "driver/pcsim.h"

static int at_init()
{

}

static int at_reset()
{

}

static int at_send()
{

}

static int at_recv()
{

}

static int at_ioctl(void *cmd)
{

}

#endif

