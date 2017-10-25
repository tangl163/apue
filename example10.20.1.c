#include "common.h"
#include <signal.h>

static void sig_int(int signo);
static int count = 0;

int
main(void)
{
    sigset_t mask, oldmask;
    struct sigaction act;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &mask, &oldmask) < 0)
        err_sys("sigprocmask error");

    printf("SIGINT has blocked\n");

    act.sa_flags = 0;
    act.sa_handler = sig_int;
    sigemptyset(&act.sa_mask);
    
    if (sigaction(SIGINT, &act, NULL) < 0)
        err_sys("sigaction error");

    printf("signal handler has installed\n");

    sleep(20);

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("set signal mask error");

    printf("SIGINT is unblocked\n");

    printf("SIGINT be caught %d times\n", count);

    exit(0);
}

static void
sig_int(int signo)
{
    count++;
    printf("signal: %d caught\n", signo);
}

