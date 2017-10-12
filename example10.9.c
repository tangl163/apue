#include "common.h"
#include <signal.h>

int
main(int argc, char *argv[])
{
    long pid;

    if (argc != 2)
        err_quit("Usage: %s pid", argv[0]);

    pid = atol(argv[1]);

    if (kill(pid, 0) < 0)
        err_ret("pid: %ld", pid);
    else
        printf("success\n");

    exit(0);
}

