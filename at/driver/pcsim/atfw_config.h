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
#define dbg_printf(fmt...)    printf((fmt))
#else
#define dbg_printf(x)    ...
#endif
}

/***************************************************************************************
 
                                    The RTOS portion

***************************************************************************************/
#include <pthread.h>

typedef enum {
    ENV_BAREMATAL,
    ENV_RTOS,
} env_t;

/**
 * get running environment
 */
static env_t os_getrunenv(void)
{
    return ENV_RTOS;
}

static int queue;

static int os_queue_init(void)
{
    return 0;
}

static int os_queue_wait(uint8_t *buf, uint32_t bufsz, uint32_t *recvsz, uint32_t timeout)
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

