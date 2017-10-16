#include "common.h"
#include <signal.h>

typedef void Sigfunc(int);

static void sig_int(int);
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
    sigset_t set;
    struct sigaction act;

    if (signo <= 0 || signo >= NSIG) {
        errno = EINVAL;
        return SIG_ERR;
    }

    sigemptyset(&set);
    sigaddset(&set, signo);

    act.sa_handler = func;

    if (sigaction(signo, &act, NULL) < 0)
        return SIG_ERR;

    return (Sigfunc *)0;
}

static void
sig_int(int signo)
{
    printf("signal: %d caught\n", signo);
}

