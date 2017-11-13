#include "common.h"
#include <setjmp.h>
#include <signal.h>

static void sig_int(int);

static jmp_buf jmpbuf;

int
main(void)
{
    if (setjmp(jmpbuf) == 0) {
        if (signal(SIGINT, sig_int) == SIG_ERR)
            err_sys("signal error");

        pr_sigmask("before pause");
        pause();
    }

    pr_sigmask("out of handler");

    exit(0);
}

static void
sig_int(int signo)
{
    printf("signal: %d caught\n", signo);
    pr_sigmask("in handler");
    longjmp(jmpbuf, 1);
}

