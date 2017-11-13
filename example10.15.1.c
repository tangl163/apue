#include "common.h"
#include <setjmp.h>
#include <signal.h>

static void sig_int(int);

static sigjmp_buf jmpbuf;
static sig_atomic_t canjmp;

int
main(void)
{
    sigset_t set, aset;

    sigemptyset(&set);
    sigemptyset(&aset);
    sigaddset(&set, SIGQUIT);
    sigaddset(&aset, SIGTSTP);

    if (sigprocmask(SIG_BLOCK, &aset, NULL) < 0)
        err_sys("sigprocmask error");

    if (sigsetjmp(jmpbuf, 1) == 0) {
        canjmp = 1;

        if (signal(SIGINT, sig_int) == SIG_ERR)
            err_sys("signal error");

        if (sigprocmask(SIG_BLOCK, &set, NULL) < 0)
            err_sys("sigprocmask error");

        pr_sigmask("before pause");
        pause();
    }

    pr_sigmask("out of handler");

    exit(0);
}

static void
sig_int(int signo)
{
    if (canjmp == 0)
        return;

    printf("signal: %d caught\n", signo);
    pr_sigmask("in handler");
    canjmp = 0;
    siglongjmp(jmpbuf, 1);
}

