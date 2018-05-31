#include "common.h"
#include <signal.h>
#include <sys/wait.h>

static void sig_trap(int);

int
main(void)
{
    int status;
    pid_t pid;
    struct sigaction act;

    act.sa_handler = sig_trap;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_INTERRUPT;

    if (sigaction(SIGINT, &act, NULL) < 0)
        err_sys("sigaction error");

    // if (signal(SIGINT, sig_trap) == SIG_ERR)
    //     err_sys("install signal for SIGINT error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        sleep(2);
        kill(getppid(), SIGINT);
        _exit(0);
    }

    if (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR)
            printf("waitpid was interrupted\n");
        else
            err_sys("waitpid error");
    } else {
        pr_exit(status);
    }
    
    exit(0);
}

static void
sig_trap(int signo)
{
    printf("signo: %d caught\n", signo);
}

