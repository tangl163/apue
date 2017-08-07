#include "common.h"
#include <signal.h>

static void sig_quit(int);
static void sig_chld(int);
static void sig_usr1(int);

int main(void)
{
    sigset_t newmask, oldmask, pendmask;

    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("can't catch SIGUSR1");
    
    while (1)
        pause();

    exit(0);
}

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
        err_sys("can't restore SIGQUIT");
}

static void sig_chld(int signo)
{
    printf("caught SIGCHLD\n");
}

static void sig_usr1(int signo)
{
    printf("caught SIGUSR1\n");
}

