#include "common.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define LOCALHOST "127.0.0.1"

static void dealrequest(int sockfd);

int
main(int argc, char *argv[])
{
    int ret, fd, port;
    uint32_t addr;
    struct sockaddr_in listenaddr;

    if (argc != 2)
        err_quit("Usage: %s port", argv[0]);

    /* avoid zombie process */
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
        err_sys("signal error");

    port = atoi(argv[1]);
    if (port > USHRT_MAX)
        err_quit("Invalid port number");

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        err_sys("socket error");

    ret = inet_pton(AF_INET, LOCALHOST, &addr);
    if (ret != 1) {
        close(fd);
        err_sys("inet_pton error");
    }

    memset(&listenaddr, 0, sizeof(struct sockaddr_in));

    listenaddr.sin_family = AF_INET;
    listenaddr.sin_port = htons((uint16_t)port);
    listenaddr.sin_addr.s_addr = addr;

    if (bind(fd, (struct sockaddr *)&listenaddr, sizeof(struct sockaddr_in)) == -1) {
        close(fd);
        err_sys("bind error");
    }

    if (listen(fd, SOMAXCONN) == -1) {
        close(fd);
        err_sys("listen error");
    }

    for ( ;; ) {
        ret = accept(fd, NULL, NULL);
        if (ret == -1) {
            close(fd);
            err_sys("accept error");
        }

        dealrequest(ret);
    }

    exit(0);
}

static void
dealrequest(int sockfd)
{
    int len;
    pid_t pid;
    char buf[BUFSIZ];

    pid = fork();
    if (pid == -1) {
        err_sys("fork error");
    } else if (pid == 0) {
        for ( ;; ) {
            len = recv(sockfd, buf, BUFSIZ, 0);
            if (len == -1)
                err_sys("recv error");

            if (len == 0) {
                printf("connection closed\n");
                break;
            }

            if (write(STDOUT_FILENO, buf, len) != len)
                err_sys("write error");
        }

        exit(0);

    } else {
        printf("Child: %d is processing the request\n", pid);
    }
}

