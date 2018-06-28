#include "common.h"
#include <sys/ipc.h>

int
main(int argc, char *argv[])
{
    int id;
    key_t key;

    if (argc != 3)
        err_quit("Usage: %s path id", argv[0]);

    id = atoi(argv[2]);

    key = ftok(argv[1], id);
    if (key < 0)
        err_sys("ftok error");

    printf("key: %d\n", (int)key);

    exit(0);
}

