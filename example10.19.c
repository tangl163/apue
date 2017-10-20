#include "common.h"
#include <signal.h>

static void sig_alrm(int signo);
static unsigned int sleep1(unsigned int seconds);

int
main(void)
{
}

static unsigned int
sleep1(unsigned int seconds)
{
    unsigned int unsleep, reminder;
    sigset_t set, set_saved, emptyset;
    struct sigaction sa_alrm, saved;

    unsleep = reminder = 0;

    sa_alrm.sa_flags = 0;
    sa_alrm.sa_handler = sig_alrm;
    sigemptyset(&sa_alrm.sa_mask);
    sigemptyset(&set);
    sigemptyset(&emptyset);
    sigaddset(&set, SIGALRM);

    if (sigprocmask(SIG_BLOCK, &set, &set_saved) < 0)
        err_sys("sigprocmask error");

    if (sigaction(SIGALRM, &sa_alrm, &saved) < 0)
        err_sys("sigaction error");

    if ((reminder = alarm(seconds)) == 0)
        unsleep = 0;
    else if (reminder >= seconds)
        unsleep = reminder - seconds;
    else
        alarm(reminder);
    
    sigsuspend(&emptyset);

    sigaction(SIGALRM, &saved, NULL);
    sigprocmask(SIG_SETMASK, &set_saved, NULL);

    alarm(unsleep);
    
    return unsleep;
}

static void
sig_alrm(int signo)
{
}

