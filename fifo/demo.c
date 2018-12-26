/*****************************************************************************************

                              a simple fifo implementation.
                       this file demonstrates how to use the fifo

*****************************************************************************************/

#include <stdio.h>
#include "fifo.h"

int main(void)
{
#define ERR_HANDLE(x) \
    if(x != F_OK) {printf("[ERROR]:@%s@%d,res=%d\n", __FUNCTION__, __LINE__, x); return -1;}

#define MY_FIFOCNT  10    /* 9 deep in fact */
typedef int USER_DATATYPE;

    USER_DATATYPE data[MY_FIFOCNT];
    struct NODE node;
    fres_t res;
    unsigned int deeptotal, deep;

    memset(&node, 0, sizeof(node));
    res = fifo_init(&node, data, sizeof(data[0]), ARR_CNT(data));
    ERR_HANDLE(res);

    res = fifo_deeptotal(&node, &deeptotal);
    ERR_HANDLE(res);
    printf("fifo_deeptotal=%d\n", deeptotal);

    res = fifo_deep(&node, &deep);
    ERR_HANDLE(res);
    printf("fifo_deep=%d\n", deep);

    // write fifo with data one by one
    {
        USER_DATATYPE testdata[MY_FIFOCNT] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        unsigned int i;

        printf("fifo in  ...\n");
        printf("write=");
        for(i = 0; i < deeptotal; i++)
        {
            res = fifo_in(&node, &testdata[i]);
            ERR_HANDLE(res);
            printf("%d ", testdata[i]);
        }
        printf("\n");
    }

    // read fifo with one by one
    {
        USER_DATATYPE read;
        unsigned int i;

        printf("fifo out ...\n");
        res = fifo_deep(&node, &deep);
        ERR_HANDLE(res);
        printf("fifo_deep=%d\n", deep);
        printf("read=");
        for(i = 0; i < deep; i++)
        {
            res = fifo_out(&node, &read);
            ERR_HANDLE(res);
            printf("%d ", read);
        }
        printf("\n");
    }

    // write fifo with many units
    {
        USER_DATATYPE testdata[] = {11, 22, 33, 44, 55, 66, 77, 88, 99, 111, 222, 333, 444, 555};
        unsigned int wc, i;

        printf("write fifo ...\n");
        res = fifo_deep(&node, &deep);
        ERR_HANDLE(res);
        printf("fifo_deep=%d\n", deep);
        printf("write=");
        for(i = 0; i < ARR_CNT(testdata); i++)
        {
            printf("%d ", testdata[i]);
        }
        printf("\n");
        res = fifo_write(&node, testdata, ARR_CNT(testdata), &wc);
        ERR_HANDLE(res);
        printf("wc=%d\n", wc);
    }

    // read fifo with many units
    {
        USER_DATATYPE buf[20] = {0};
        unsigned int rc, i;

        printf("read fifo ...\n");
        res = fifo_deep(&node, &deep);
        ERR_HANDLE(res);
        printf("fifo_deep=%d\n", deep);
        res = fifo_read(&node, buf, deep, &rc);
        ERR_HANDLE(res);
        printf("read=");
        for(i = 0; i < rc; i++)
        {
            printf("%d ", buf[i]);
        }
        printf("\n");
        printf("rc=%d\n", rc);
    }

    // writel fifo with many units
    {
        USER_DATATYPE testdata[] = {99, 88, 77, 66, 55, 44, 33, 22, 11, 111, 222, 333, 444, 555};
        unsigned int wc, i;

        printf("writel fifo ...\n");
        res = fifo_deep(&node, &deep);
        ERR_HANDLE(res);
        printf("fifo_deep=%d\n", deep);
        printf("writel=");
        for(i = 0; i < ARR_CNT(testdata); i++)
        {
            printf("%d ", testdata[i]);
        }
        printf("\n");
        res = fifo_writel(&node, testdata, ARR_CNT(testdata), &wc);
        ERR_HANDLE(res);
        printf("wc=%d\n", wc);
    }

    // read fifo with many units
    {
        USER_DATATYPE buf[20] = {0};
        unsigned int rc, i;

        printf("read fifo ...\n");
        res = fifo_deep(&node, &deep);
        ERR_HANDLE(res);
        printf("fifo_deep=%d\n", deep);
        res = fifo_read(&node, buf, deep, &rc);
        ERR_HANDLE(res);
        printf("read=");
        for(i = 0; i < rc; i++)
        {
            printf("%d ", buf[i]);
        }
        printf("\n");
        printf("rc=%d\n", rc);
    }

    return 0;
}
