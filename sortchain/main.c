#include <stdio.h>
#include <math.h>
#include "sortchain.h"

#define PI          (3.1415926)
#define ARRAY_CNT   (40)
#define ERR_PRT()   printf("error in line: %d\n", __LINE__)

static void get_sin_array(schdat_t *p, int len, int period)
{
    int i;
    int k = 2 * PI / period;

    for(i = 0; i < len; i++)
    {
        p[i] = 10 * sin(k * i); 
    }
}

int main(void)
{
    schh_t datalib;
    schres_t res;
    schdat_t buf [ARRAY_CNT];
    schdat_t buf1[ARRAY_CNT];
    schdat_t buf2[ARRAY_CNT];
    schdat_t mid;
    int i;
    int ofs = 98;
    int period = 8;
    int dummy;

    printf("start...\n");
    // prepare the resource array: buf1, buf2
    get_sin_array(buf1, ARRAY_CNT, period);
    for(i = 0; i < ARRAY_CNT; i++)
    {
        buf2[i] = buf1[i] + ofs;
    }

    // filt the buf1 to buf
    if(sortchain_init(&datalib, period) != SCHRES_OK)
    {
        ERR_PRT();
        return -1;
    }
    for(dummy = 0, i = 0; i < ARRAY_CNT; i++)
    {
        res = sortchain_add(&datalib, buf1[i], &mid);
        if(res == SCHRES_NOTREADY)
        {
            dummy++;
        }
        else if(res == SCHRES_ERR)
        {
            ERR_PRT();
            return -2;
        }
        buf[i] = buf1[i] - mid;
    }

    // now the buf is filted, verify it with buf2
    for(i = dummy; i < ARRAY_CNT; i++)
    {
        if(buf[i] != buf2[2] - ofs)
        {
            ERR_PRT();
            return -3;
        }
    }

    printf("ok\n");
    return 0;
}

