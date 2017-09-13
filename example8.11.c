#include "common.h"

int main(int argc, char *argv[])
{
    pid_t pid;

    if (argc != 3)
        err_quit("Usage: %s <name> <name>", argv[0]);

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        execlp("echoall.sh", argv[1], argv[2], (char *)0);
        err_sys("execl error");
    }

    exit(0);
}

