/*******************************************************************************

                             tail, a trace tool
                            this a demo for tail

*******************************************************************************/

#include "tail.h"
#include <stdint.h>

int func(void)
{
    if(1 + 1 == 2)
    {
        return 0;
    }
    return -1;
}

int main(void)
{
    int res;

    tail_init();
    tail_pin(TAIL_PRT_SAV, "info", "tail_init");

    res = func();
    tail_pin(res, "err", "an error occure, this is just a demo, don't worry");

    tail_emerge("info", "the demo runs up");

    tail_pin(TAIL_SAV_ONLY, "warn", "i just want to");

    tail_printall();

    return 0;
}
