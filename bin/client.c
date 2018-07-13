#include "common.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXSLEEP 128

static int connet_retry(int domain, int type, int protocol,
                        const struct sockaddr *addr, socklen_t len);

int
main(int argc, char *argv[])
{
    int fd;
    int ret, port;
    uint32_t addr;
    char buf[BUFSIZ];
    struct sockaddr_in serveraddr;

    if (argc != 3)
        err_quit("Usage: %s ip port", argv[0]);

    port = atoi(argv[2]);
    if (port > USHRT_MAX)
        err_quit("Invalid port");

    ret = inet_pton(AF_INET, argv[1], &addr);
    if (ret == -1)
        err_quit("inet_pton error");

    if (ret == 0)
        err_quit("Invalid address: %s", argv[1]);

    memset(&serveraddr, 0, sizeof(struct sockaddr_in));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons((uint16_t)port);
    serveraddr.sin_addr.s_addr = addr;

    fd = connet_retry(AF_INET, SOCK_STREAM, 0, (struct sockaddr *)&serveraddr,
                      sizeof(struct sockaddr_in));
    if (fd == -1)
        err_sys("connect error");

    for ( ;; ) {
        ret = read(STDIN_FILENO, buf, BUFSIZ);
        if (ret == -1) {
            shutdown(fd, SHUT_RDWR);
            err_sys("read error");
        }

        if (ret == 0) {
            shutdown(fd, SHUT_RDWR);
            printf("EOF read");
        }

        if (send(fd, buf, ret, 0) == -1) {
            shutdown(fd, SHUT_RDWR);
            err_sys("send error");
        }
    }

    exit(0);
}

static int
connet_retry(int domain, int type, int protocol,
             const struct sockaddr *addr, socklen_t len)
{
    int fd, num;

    for (num = 1; num <= MAXSLEEP; num <<= 1) {
        fd = socket(domain, type, protocol);
        if (fd == -1)
            return -1;

        if (connect(fd, addr, len) == 0)
            return fd;

        close(fd);

        if (num <= MAXSLEEP / 2)
            sleep(num);
    }

    return -1;
}

