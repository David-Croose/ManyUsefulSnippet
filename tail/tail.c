/*******************************************************************************

                             tail, a trace tool

*******************************************************************************/

#include "fifo.h"
#include "tail.h"
#include <string.h>
#include <stdio.h>

// the local variables
static struct NODE fifohdl;                              // fifo handle
static char fifobuf[TAIL_BUFTOT + 1][TAIL_BUFSIZE];      // fifo buffer

/**
 * tail initialization
 */
void tail_init(void)
{
    TAIL_CRITICAL_ENTER();
    memset(&fifohdl, 0, sizeof(fifohdl));
    memset(fifobuf, 0, sizeof(fifobuf));
    if(fifo_init(&fifohdl, fifobuf, sizeof(fifobuf[0]), sizeof(fifobuf) / sizeof(fifobuf[0])) != F_OK)
    {
        TAIL_PRINTF("[err] tail_init failed!\r\n");
    }
    TAIL_CRITICAL_EXIT();
}

/**
 * print an output string to terminal or local memory
 * @param opt : the option controls where the output string should go to, see @tailopt_t
 * @param tag : the tag of output string
 * @param info: the main infomation of the output string
 * @param file: in which file this function was called, if this parameter is NULL
 *              the @func, @line and itself will not be included into output string
 * @param func: the name of function calls this function
 * @param line: the line number where this function was called
 */
void tail_print(tailopt_t opt, char *tag, char *info,
                const char *file, const char *func, uint32_t line)
{
    static char buf[TAIL_BUFSIZE];
    const char *_tag = tag;
    const char *_info = info;

    TAIL_CRITICAL_ENTER();

    if(!tag)
    {
        _tag = "warn";
    }
    if(!info)
    {
        _info = "---";
    }

    memset(buf, 0, sizeof(buf));
    if(file)
    {
        snprintf(buf, sizeof(buf), "[%s] %s, in \"file:%s,func:%s,line:%d\"",
                 _tag, _info, file, func, line);
    }
    else
    {
        snprintf(buf, sizeof(buf), "[%s] %s", _tag, _info);
    }

    switch(opt)
    {
    case TAIL_NONE:
        break;

    case TAIL_SAV_ONLY:
        fifo_in(&fifohdl, buf);
        break;

    case TAIL_PRT_ONLY:
        TAIL_PRINTF("%s\r\n", buf);
        break;

    case TAIL_PRT_SAV:
    default:
        fifo_in(&fifohdl, buf);
        TAIL_PRINTF("%s\r\n", buf);
        break;
    }

    TAIL_CRITICAL_EXIT();
}

/**
 * print all output string in the tail queue
 */
void tail_printall(void)
{
    uint32_t deeptotal, deep;
    static char buf[TAIL_BUFSIZE];

    fifo_deeptotal(&fifohdl, &deeptotal);
    fifo_deep(&fifohdl, &deep);

    TAIL_CRITICAL_ENTER();

    TAIL_PRINTF("\r\n====================== tail_printall ======================\r\n");
    TAIL_PRINTF("[info] tail_total_buffer=%d\r\n", deeptotal);
    TAIL_PRINTF("[info] tail_buffer_size=%d\r\n", TAIL_BUFSIZE);
    TAIL_PRINTF("[info] tail_used_buffer=%d\r\n", deep);
    TAIL_PRINTF("\r\n");
    while(fifo_out(&fifohdl, buf) == F_OK)
    {
        TAIL_PRINTF("%s\r\n", buf);
    }
    TAIL_PRINTF("--------------------------------------------------------------\r\n");

    TAIL_CRITICAL_EXIT();
}
