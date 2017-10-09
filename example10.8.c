#include "common.h"
#include <signal.h>

static void sig_int(int);

int
main(void)
{
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("install signal for SIGINT error");

    pause();

    exit(0);
}

static void
sig_int(int signo)
{
    printf("signo: %d caught\n", signo);
    sleep(2);
}

