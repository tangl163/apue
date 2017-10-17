#include "../common.h"
#include <signal.h>

static sigset_t blockset;

void
TELL_WAIT(int signo)
{
    sigfillset(&blockset);
    sigdelset(&blockset, signo);
    sigsuspend(&blockset);
}

void
TELL_PARENT(int signo)
{
    kill(getppid(), signo);
}

void
TELL_CHILD(pid_t pid, int signo)
{
    kill(pid, signo);
}

void
WAIT_CHILD(int signo)
{
    sigfillset(&blockset);
    sigdelset(&blockset, signo);
    sigsuspend(&blockset);
}

void
WAIT_PARENT(int signo)
{
    sigfillset(&blockset);
    sigdelset(&blockset, signo);
    sigsuspend(&blockset);
}

