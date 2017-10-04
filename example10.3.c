#include "common.h"
#include <signal.h>

void sig_quit(int);

int
main(void)
{
    pid_t pid;

    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
        err_sys("signal for SIGINT error");

    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        err_sys("signal for SIGQUIT error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        // The pause program just pause waiting for signals to come.
        execl("/home/paul/apue/bin/pause", "pause", (char *)0);
        exit(0);
    }

    exit(0);
}

void sig_quit(int signo)
{
    printf("signal %d caught\n", signo);
}

