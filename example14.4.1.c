#include "common.h"
#include <signal.h>
#include <sys/select.h>

static void sig_int(int);

int
main(void)
{
    int ret;
    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    timeout.tv_sec = 5;
    timeout.tv_usec = 50;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    ret = select(4, &readfds, NULL, NULL, &timeout);
    if (ret > 0)
        printf("STDIN_FILENO is ready\n");
    else if (ret == 0)
        printf("select timeout\n");
    else if (ret == -1 && errno == EINTR)
        printf("tv_sec: %ld tv_usec: %ld\n", timeout.tv_sec, timeout.tv_usec);
    else
        err_sys("select error");

    printf("ret: %d\n", ret);
    exit(0);
}

static void
sig_int(int signo)
{
    printf("signo: %d caught\n", signo);
}

