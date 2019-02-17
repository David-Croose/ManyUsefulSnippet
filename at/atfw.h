/***************************************************************************************

                                    AT-Frame-Work
                     an universal AT instruction driver frame work

***************************************************************************************/
#ifndef _ATFW_H_
#define _ATFW_H_

#include <stdint.h>


typedef enum {
    ATFW_OK = 0,
    ATFW_ERR,
} atfwres_t;

typedef int32_t cb_t(void *);


#endif

