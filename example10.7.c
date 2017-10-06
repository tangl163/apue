#include "common.h"
#include <signal.h>
#include <sys/wait.h>

//static void sig_chld(int);

int
main(void)
{
    pid_t pid;

    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
        err_sys("install signal handle for SIGCHLD error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        _exit(0);    // first child
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        sleep(10);    // second child
    }

    if (wait(NULL) < 0)
        err_sys("wait error");

    exit(0);
}

