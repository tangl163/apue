#include "common.h"
#include <setjmp.h>
#include <signal.h>

static void sig_abrt(int);
static sig_atomic_t canjmp;
static sigjmp_buf sigjmpbuf;

int
main(void)
{
    if (signal(SIGABRT, sig_abrt) == SIG_ERR)
        err_sys("signal error");

    if (sigsetjmp(sigjmpbuf, 1))
        err_quit("exit normally");

    canjmp = 1;

    abort();

    exit(0);
}

static void
sig_abrt(int signo)
{
    if (canjmp == 0)
        return;

    siglongjmp(sigjmpbuf, signo);
}

