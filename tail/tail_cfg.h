/*******************************************************************************

                             tail, a trace tool

                       this is the configuration file

*******************************************************************************/
#ifndef _TAIL_CFG_H_
#define _TAIL_CFG_H_

// one buffer size(in bytes) of tail
#define TAIL_BUFSIZE                              (100)

// total buffer counts of tail
#define TAIL_BUFTOT                               (10)

// the print driver
// like #define TAIL_PRINTF(...) \
// printf(__VA_ARGS__)
#define TAIL_PRINTF(...)                          printf(__VA_ARGS__)

// tail may need to be protected during
// it's processing
#define TAIL_CRITICAL_ENTER()
#define TAIL_CRITICAL_EXIT()

#endif
