#include "common.h"
#include <signal.h>
#include <sys/wait.h>

void sig_chld(int);

int
main(void)
{
    int status;
    pid_t pid;

    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        err_sys("install signal for SIGCHLD error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        sleep(2);
        _exit(0);
    }

    if (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR)
            printf("waitpid was interrupted\n");
    } else {
        pr_exit(status);
    }
    
    exit(0);
}

void
sig_chld(int signo)
{
    printf("signo: %d caught\n", signo);
    
    if (waitpid(-1, NULL, 0) < 0)
        err_ret("waitpid error");
}

