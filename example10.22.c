#include "common.h"

int
main(int argc, char *argv[])
{
    int signo;

    if (argc != 2)
        err_quit("Usage: %s signo", argv[0]);

    signo = atoi(argv[1]);

    printf("%s\n", strsignal(signo));

    exit(0);
}

