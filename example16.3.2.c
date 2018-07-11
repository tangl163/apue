#include "common.h"
#include <netinet/in.h>
#include <arpa/inet.h>

int
main(int argc, char *argv[])
{
    int ret;
    unsigned char buf[4];

    if (argc != 2)
        err_quit("Usage: %s ip", argv[0]);

    ret = inet_pton(AF_INET, argv[1], (void *)buf);
    if (ret == -1)
        err_sys("inet_pton error");

    if (ret == 0)
        err_quit("Invalid format");

    printf("%hhu.%hhu.%hhu.%hhu\n", buf[0], buf[1], buf[2], buf[3]);

    exit(0);
}

