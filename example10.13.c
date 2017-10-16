#include "common.h"
#include <signal.h>

static void sig_quit(int);

int
main(void)
{
    sigset_t newset, oldset, pendingset;
    
    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        err_sys("install signal handler for SIGQUIT error");

    sigemptyset(&newset);
    sigaddset(&newset, SIGQUIT);
    
    if (sigprocmask(SIG_SETMASK, &newset, &oldset) < 0)
        err_sys("sigprocmask error");

    sleep(5);

    if (sigpending(&pendingset) < 0)
        err_sys("sigpending error");

    if (sigismember(&pendingset, SIGQUIT))
        printf("SIGQUIT is pending\n");

    if (sigprocmask(SIG_SETMASK, &oldset, NULL) < 0)
        err_sys("restore sigprocmask error");

    printf("SIGQUIT is unblocked\n");

    exit(0);
}

static void
sig_quit(int signo)
{
    printf("SIGQUIT: %d caught\n", signo);
}

