#include <stdio.h>
#include "sortchain.h"

int main(void)
{
    schres_t res;
    schh_t datalib;
    schdat_t mid;
    unsigned int i;
    schdat_t dat[] = {
        5, 10, 9, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 17, 17, -13, 1, 9,
        19, 4, -7, 16, 9, 9, 9, 9, 13, 9, -14, 16, 7, 7, 3, -5, 11,
    };

    if(sortchain_init(&datalib, 8) != SCHRES_OK)
    {
        printf("init error!\n");
        return -1;
    }

    for(i = 0; i < sizeof(dat) / sizeof(dat[0]); i++)
    {
        res = sortchain_add(&datalib, dat[i], &mid);
        if(res == SCHRES_OK)
        {
            printf("%d ", mid);
        }
        else if(res == SCHRES_NOTREADY)
        {
            printf("-");
        }
        else
        {
            printf("run error!\n");
            return -2;
        }
    }
    return 0;
}

