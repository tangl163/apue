#include "common.h"
#include <signal.h>

static void sig_int(int);

int
main(void)
{
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    pause();

    exit(0);
}

static void
sig_int(int signo)
{
    printf("signal: %d caught\n", signo);
    pr_sigmask("in signal handle");
}

