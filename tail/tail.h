/*******************************************************************************

                             tail, a trace tool

*******************************************************************************/

#ifndef _TAIL_H_
#define _TAIL_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TAIL_NONE = 0,       // do nothing
    TAIL_PRT_SAV,        // print and save
    TAIL_PRT_ONLY,       // print only
    TAIL_SAV_ONLY,       // save only
} tailopt_t;             // tail option typedef

// the exported functions

/**
 * pin an output string to terminal or local memory. this function is a simple
 * version of @tail_print, it gave itself the @file, @func and @line parameters
 * @param opt : the option controls where the output string should go to, see @tailopt_t
 * @param tag : the tag of output string
 * @param fmt... : the formatted string
 */
#define tail_pin(opt, tag, fmt...)    tail_print((opt), (tag), (__FILE__), (__FUNCTION__), (__LINE__), ## fmt)

/**
 * just print an format string to terminal. this function is a simple version of @tail_print
 * it just prints the @tag and @fmt, and doesn't save anything to memory
 * @param tag : the tag of output string
 * @param fmt... : the formatted string
 */
#define tail_emerge(tag, fmt...)      tail_print((TAIL_PRT_ONLY), (tag), 0, 0, 0, ## fmt)

void tail_init(void);
void tail_print(tailopt_t opt, const char *tag, const char *file, const char *func,
                uint32_t line, const char *fmt, ...);
void tail_printall(void);

#include "tail_cfg.h"

#endif
