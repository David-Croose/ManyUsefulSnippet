#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "atfw.h"

int main(void)
{
    uint8_t recvbuf[100];
    uint8_t sendbuf[100];
    uint32_t sendlen;
    uint32_t recvlen;
    uint32_t i;
    uint32_t line;

    printf("demo starts\n");

    // initialize the atfw
    if (line = __LINE__, atfw_init()) {
        goto out;
    }
    printf("atfw_init() OK\n");

    // send AT command
    if (line = __LINE__, atfw_sendwaitstr("AT: init\n", "OK\n", 0, 0, 100)) {
        goto out;
    }
    printf("atfw send \"AT: init\\n\" OK\n");

    // send AT command
    if (line = __LINE__, atfw_sendwaitstr("AT: open\n", "OK\n", 0, 0, 100)) {
        goto out;
    }
    printf("atfw send \"AT: open\\n\" OK\n");

    // send AT command
    if (line = __LINE__, atfw_sendwaitstr("AT: status\n", "OK\n", recvbuf, sizeof(recvbuf), 100)) {
        goto out;
    }
    printf("atfw send \"AT: status\\n\" OK\n");
    printf("the status: %s\n", (char *)recvbuf + strlen("OK\n"));

    // send AT send-data-command, the AT-device will wait for the next input data to send
    if (line = __LINE__, atfw_sendwaitstr("AT: send\n", "OK, waiting for input\n", 0, 0, 100)) {
        goto out;
    }
    printf("atfw send \"AT: send\\n\" OK\n");
    sendbuf[0] = 0x11;
    sendbuf[1] = 0x22;
    sendbuf[2] = 0x33;
    sendbuf[3] = 0x44;
    sendbuf[4] = 0x55;
    sendlen = 5;
    if (line = __LINE__, atfw_sendwaitbuf(sendbuf, sendlen, recvbuf, sizeof(recvbuf), &recvlen, 100)) {
        goto out;
    }
    printf("sendbuf: \n");
    for (i = 0; i < sendlen; i++) {
        printf("%02x ", sendbuf[i]);
    }
    printf("\nhas been sent out, and what we receive is:\n");
    for (i = 0; i < recvlen; i++) {
        printf("%02x ", recvbuf[i]);
    }
    printf("\n");

    sendbuf[0] = 0xAA;
    sendbuf[1] = 0xBB;
    sendbuf[2] = 0xCC;
    sendbuf[3] = 0xDD;
    sendbuf[4] = 0xEE;
    sendbuf[5] = 0xFF;
    sendlen = 6;
    if (line = __LINE__, atfw_sendwaitbuf(sendbuf, sendlen, recvbuf, sizeof(recvbuf), &recvlen, 100)) {
        goto out;
    }
    printf("sendbuf: \n");
    for (i = 0; i < sendlen; i++) {
        printf("%02x ", sendbuf[i]);
    }
    printf("\nhas been sent out, and what we receive is:\n");
    for (i = 0; i < recvlen; i++) {
        printf("%02x ", recvbuf[i]);
    }
    printf("\n");

    // send AT command
    if (line = __LINE__, atfw_sendwaitstr("AT: close\n", "OK\n", 0, 0, 100)) {
        goto out;
    }
    printf("atfw send \"AT: close\\n\" OK\n");

    printf("the demo ends successfully\n");
    return 0;

out:
    printf("error happened in line:%d!\n", line);
    return -1;
}

