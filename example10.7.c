#include "common.h"
#include <signal.h>
#include <sys/wait.h>

static void sig_chld(int);

int
main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        _exit(0);
    }

    sleep(2);
    
    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        err_sys("install signal handle for SIGCHLD error");

    exit(0);
}

static void
sig_chld(int signo)
{
    printf("Caught signal: %d\n", signo);

    if (waitpid(-1, NULL, 0) < 0)
        err_sys("waitpid error");
}

