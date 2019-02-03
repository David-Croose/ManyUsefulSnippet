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

typedef struct {


};

typedef int32_t cb_t(void *);

typedef struct {
    uint32_t id;
    char name[20];
    void (*send)();
    void (*recv)();
    void (*delay)();
    void (*wait)();
    void *extra;
} atfwhdl_t;    // AT-Frame-Work Handle



#endif


