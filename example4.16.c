#include "common.h"


int main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <filename>", argv[0]);

    if (unlink(argv[1]) < 0)
        err_sys("unlink error");

    printf("done\n");

    exit(0);
}

