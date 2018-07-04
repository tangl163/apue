#include "common.h"
#include <sys/wait.h>

/**
 * 14.7 Determine the capacity of a pipe using nonblocking writes.
 * Compare this value with the value of PIPE_BUF from Chapter 2.
 */
int
main(void)
{
    int n, fd[2];
    long capacity;

    if (pipe(fd) < 0)
        err_sys("pipe error");

    set_fl(fd[1], O_NONBLOCK);

    for (capacity = 0; /* void */ ; capacity++) {
        n = write(fd[1], "a", 1);
        if (n != 1) {
            printf("write return %d\n", n);
            break;
        }
    }

    printf("Capacity: %ld\n", capacity);
    printf("PIPE_BUF: %d\n", PIPE_BUF);

    exit(0);
}

