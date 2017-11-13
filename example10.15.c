#include "common.h"
#include <signal.h>
#include <sys/wait.h>

static void sig_int(int);

int
main(void)
{
    pid_t pid;
    struct sigaction act;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {

        /* We ignore SIGINT in the child. */
        if (signal(SIGINT, SIG_IGN) == SIG_ERR)
            err_sys("signal error");

        sleep(15);
        exit(0);
    }
    
    act.sa_flags = 0;
    //act.sa_flags |= SA_RESTART;
    act.sa_handler = sig_int;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGINT, &act, NULL) < 0)
        err_sys("sigaction error");

    /* We test if waitpid could be interrupted or not. */
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");

    exit(0);
}

static void
sig_int(int signo)
{
    printf("signal: %d caught\n", signo);
    pr_sigmask("in signal handle");
}

