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
 * pin an output string to terminal or local memory. this function is the simple
 * version of @tail_print, it gave itself the @file, @func and @line parameters
 * @param opt : the option controls where the output string should go to, see @tailopt_t
 * @param tag : the tag of output string
 * @param info: the main infomation of the output string
 */
#define tail_pin(opt, tag, info)    tail_print((opt), (tag), (info), __FILE__, __FUNCTION__, __LINE__)

/**
 * just print an output string to terminal or local memory. this function is the simple
 * version of @tail_print, it just prints the @tag and @info, and doesn't save anything
 * @param opt : the option controls where the output string should go to, see @tailopt_t
 * @param tag : the tag of output string
 * @param info: the main infomation of the output string
 */
#define tail_emerge(tag, info)      tail_print((TAIL_PRT_ONLY), (tag), (info), 0, 0, 0)

void tail_init(void);
void tail_print(tailopt_t opt, char *tag, char *info, const char *file, const char *func, uint32_t line);
void tail_printall(void);

#include "tail_cfg.h"

#endif
