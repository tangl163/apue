#include "common.h"
#include <signal.h>
#include <poll.h>

static void sig_int(int signo);

int
main(void)
{
    int ret;
    struct pollfd pd;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    pd.fd = STDIN_FILENO;
    pd.events = POLLIN;

    ret = poll(&pd, 1, 2000);
    if (ret > 0)
        printf("STDIN_FILENO ready\n");
    else if (ret == 0)
        printf("poll timeout\n");
    else if (ret < 0 && errno == EINTR)
        printf("Interrupted by signal\n");
    else
        err_sys("poll error");

    exit(0);
}

static void
sig_int(int signo)
{
    printf("signo: %d caught\n", signo);
}

