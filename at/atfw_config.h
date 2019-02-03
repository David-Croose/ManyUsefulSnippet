/***************************************************************************************

                                    AT-Frame-Work
                     an universal AT instruction driver frame work

***************************************************************************************/

#ifndef _ATFW_CONFIG_H_
#define _ATFW_CONFIG_H_


/***************************************************************************************
 
                                    The configuration

***************************************************************************************/
#define CFG_VERSION             "V1"
#define CFG_DEBUG_ENABLE        (1)
#define CFG_RECV_BUF_SIZE       (1600)

/***************************************************************************************
 
                                    The debug portion

***************************************************************************************/
#if (CFG_DEBUG_ENABLE == 1)
static int dbg_print(const char *fmt, ...)
{


}
#else
#define dbg_print(x)    ...
#endif
}

/***************************************************************************************
 
                                    The RTOS portion

***************************************************************************************/
#include "FreeRTOS.h"

/**
 * get running environment
 * @return: 0 --- RTOS
 *          1 --- BareMetal
 */
static int os_getrunenv(void)
{


}

static int os_delay()
{

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
#include "simcon.h"

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


#endif

