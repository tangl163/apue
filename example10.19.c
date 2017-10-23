#include "common.h"
#include <signal.h>

#define IN  1
#define OUT 2

static void sig_alrm(int signo);
static unsigned int sleep1(unsigned int seconds);

int
main(int argc, char *argv[])
{
    unsigned int seconds, unsleep;

    if (argc != 2)
        err_quit("Usage: %s seconds", argv[0]);

    seconds = atoi(argv[1]);

    alarm(10);
    unsleep = sleep1(seconds);

    printf("unsleep: %u\n", unsleep);
    printf("left alarm: %u\n", alarm(0));

    exit(0);
}

static unsigned int
sleep1(unsigned int seconds)
{
    unsigned int flag, unsleep, reminder;
    sigset_t set, set_saved, suspendset;
    struct sigaction sa_alrm, saved;

    unsleep = reminder = flag = 0;

    sa_alrm.sa_flags = 0;
    sa_alrm.sa_handler = sig_alrm;
    sigemptyset(&sa_alrm.sa_mask);
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);

    if (sigprocmask(SIG_BLOCK, &set, &set_saved) < 0)
        err_sys("sigprocmask error");

    if (sigaction(SIGALRM, &sa_alrm, &saved) < 0)
        err_sys("sigaction error");

    if ((reminder = alarm(seconds)) > seconds) {
        flag = OUT;
    } else if (reminder < seconds && reminder != 0) {
        flag = IN;
        alarm(reminder);
    }
    
    suspendset = set_saved;
    sigdelset(&suspendset, SIGALRM);

    sigsuspend(&suspendset);
    unsleep = alarm(0);

    sigaction(SIGALRM, &saved, NULL);
    sigprocmask(SIG_SETMASK, &set_saved, NULL);

    if (flag == OUT)
        alarm(reminder - seconds + unsleep);
    else if (flag == IN)
        unsleep = seconds - reminder + unsleep;
    
    return unsleep;
}

static void
sig_alrm(int signo)
{
    printf("signo: %d caught\n", signo);
}

