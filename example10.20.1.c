#include "common.h"
#include <signal.h>

static void sig_handle(int signo, siginfo_t *info, void *context);
static int count = 0;

int
main(void)
{
    sigset_t mask, oldmask;
    struct sigaction act;

    sigemptyset(&mask);
    sigaddset(&mask, SIGRTMIN);

    if (sigprocmask(SIG_BLOCK, &mask, &oldmask) < 0)
        err_sys("sigprocmask error");

    printf("SIGRTMIN has blocked\n");

    act.sa_flags = 0;
    act.sa_flags |= SA_SIGINFO;
    act.sa_sigaction = sig_handle;
    sigemptyset(&act.sa_mask);
    
    if (sigaction(SIGRTMIN, &act, NULL) < 0)
        err_sys("sigaction error");

    printf("signal handler has installed\n");

    sleep(20);

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("set signal mask error");

    printf("SIGRTMIN is unblocked\n");

    printf("SIGRTMIN be caught %d times\n", count);

    exit(0);
}

static void
sig_handle(int signo, siginfo_t *info, void *context)
{
    count++;
    printf("signal: %d caught\n", signo);
}

