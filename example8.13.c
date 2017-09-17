#include "common.h"
#include <sys/wait.h>

int mysystem(char *);

int main(int argc, char *argv[])
{
    int ret;

    if (argc != 2)
        err_quit("Usage: %s <cmd>", argv[0]);

    if ((ret = mysystem(argv[1])) < 0)
        err_sys("mysystem error");

    pr_exit(ret);

    exit(ret);
}

int mysystem(char *cmdstring)
{
    pid_t pid;
    int status;

    if (cmdstring == NULL)
        return 1;

    if ((pid = fork()) < 0) {
        status = -1;
    } else if (pid == 0) {
        execlp(cmdstring, cmdstring, (char *)0);
        _exit(127);
    }

    if (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR)
            errno = 0;
        else
            status = -1;
    }

    return status;
}

