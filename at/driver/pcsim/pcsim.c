#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "atfw.h"

void *irq_handler(void *p)
{
    
    pthread_exit(0);
}


int main(void)
{
    uint8_t buf[100];
    uint8_t sendbuf[100];
    uint32_t len;
    uint32_t recvlen;
    uint32_t i;
    pthread_t thread;
         
    if (pthread_create(&thread, 0, irq_handler, 0)) {
        goto out;
    }

    if (atfw_init()) {
        goto out;
    }

    if (atfw_sendwait("AT: init\n", strlen("AT: init\n"), "OK\n", 0, 0, 100)) {
        goto out;
    }

    if (atfw_sendwait("AT: open\n", strlen("AT: open\n"), "OK\n", 0, 0, 100)) {
        goto out;
    }

    if (atfw_sendwait("AT: status\n", strlen("AT: status\n"), "OK\n", buf, sizeof(buf), 100)) {
        goto out;
    }
    printf("the status: %s\n", buf);

    if (atfw_sendwait("AT: send\n", strlen("AT: send\n"), "OK, waiting\n", 0, 0, 100)) {
        goto out;
    }

    sendbuf[0] = 0x11;
    sendbuf[1] = 0x22;
    sendbuf[2] = 0x33;
    sendbuf[3] = 0x44;
    sendbuf[4] = 0x55;
    len = 5;
    if (atfw_sendwait((const uint8_t *)sendbuf, len, 0, buf, sizeof(buf), 100)) {
        goto out;
    }
    printf("sendbuf: \n");
    for (i = 0; i < len; i++) {
        printf("%02x ", sendbuf[i]);
    }
    printf("has been sent out, and what we receive is:\n");
    for (i = 0; i < len; i++) {
        printf("%02x ", sendbuf[i]);
    }


    sendbuf[0] = 0xAA;
    sendbuf[1] = 0xBB;
    sendbuf[2] = 0xCC;
    sendbuf[3] = 0xDD;
    sendbuf[4] = 0xEE;
    sendbuf[5] = 0xFF;
    if (atfw_sendwait((const uint8_t *)sendbuf, 5, 0, buf, 5, 100)) {
        goto out;
    }
    printf("sendbuf has been sent out\n");


    if (atfw_sendwait("AT: close\n", strlen("AT: close\n"), "OK\n", 0, 0, 100)) {
        goto out;
    }

    printf("the demo runs successfully\n");
    return 0;

out:
    printf("error happened!\n");
    pthread_exit(0);
    return -1;
}

