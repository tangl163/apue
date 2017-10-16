#include "common.h"
#include <signal.h>

typedef void Sigfunc(int);

static Sigfunc sig_int;
static Sigfunc *mysignal(int, Sigfunc *);

int
main(void)
{
    if (mysignal(SIGINT, sig_int) == SIG_ERR)
        err_sys("install SIGINT error");

    pause();

    exit(0);
}

static Sigfunc *
mysignal(int signo, Sigfunc *func)
{
    struct sigaction act, oact;

    if (signo <= 0 || signo >= NSIG) {
        errno = EINVAL;
        return SIG_ERR;
    }

    act.sa_flags = 0;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);

    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
        act.sa_flags |= SA_RESTART;
    }

    if (sigaction(signo, &act, &oact) < 0)
        return SIG_ERR;

    return oact.sa_handler;
}

static void
sig_int(int signo)
{
    printf("signal: %d caught\n", signo);
}

