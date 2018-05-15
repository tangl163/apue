#include "common.h"

int
main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <pathname>", argv[0]);

    if (execl(argv[1], "hehe", (char *)0) < 0)
        err_sys("errno: %d\n", errno);

    exit(0);
}

