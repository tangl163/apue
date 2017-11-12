#include "common.h"
#include <signal.h>

static void sig_int(int);
static void sig_usr1(int);

int
main(void)
{
    sigset_t oldmask, mask;

    /* We first ignore SIGUSR1. */
    if (signal(SIGUSR1, SIG_IGN) == SIG_ERR)
        err_sys("ignore SIGUSR1 error");

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &mask, &oldmask) < 0)
        err_sys("sigprocmask error");

    pr_sigmask("before pause: ");

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("install signal for SIGINT error");

    pause();
    pr_sigmask("after pause: ");

    /* Install signal handler. */
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");

    /* Unblock SIGUSR1. */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("sigprocmask error");

    exit(0);
}

static void
sig_int(int signo)
{
    printf("signo: %d caught\n", signo);
    pr_sigmask("In sig_int: ");
}

static void
sig_usr1(int signo)
{
    printf("signo: %d caught\n", signo);
}

