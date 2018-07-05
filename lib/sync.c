#include "common.h"
#include <signal.h>

static void sig_usr(int signo);

static volatile sig_atomic_t sigflag;
static sigset_t blockset, emptyset, oldset;

static void
sig_usr(int signo)
{
    if (signo == SIGUSR1 || signo == SIGUSR2)
        sigflag = 1;
}

void
TELL_WAIT(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");

    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR2) error");

    sigemptyset(&blockset);
    sigemptyset(&emptyset);
    sigaddset(&blockset, SIGUSR1);
    sigaddset(&blockset, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &blockset, &oldset) < 0)
        err_sys("sigprocmask error");
}

void
TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);
}

void
TELL_PARENT(void)
{
    kill(getppid(), SIGUSR2);
}

void
WAIT_PARENT(void)
{
    while (sigflag == 0)
        sigsuspend(&emptyset);

    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldset, NULL) < 0)
        err_sys("restore sigmask error");
}

void
WAIT_CHILD(void)
{
    while (sigflag == 0)
        sigsuspend(&emptyset);

    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldset, NULL) < 0)
        err_sys("restore sigmask error");
}

