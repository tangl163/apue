#include "common.h"

int
main(void)
{
    int n, fd[2];
    char buf[PIPE_BUF];
    struct stat stbuf;

    if (pipe(fd) == -1)
        err_sys("pipe error");

    for ( ;; ) {
        n = read(STDIN_FILENO, buf, PIPE_BUF);
        if (n == -1)
            err_sys("read from stdin error");

        if (n == 0) {
            fprintf(stderr, "EOF read\n");
            break;
        }

        if (write(fd[1], buf, n) != n)
            err_sys("write to pipe error");

        if (fstat(fd[0], &stbuf) == -1)
            err_sys("fstat error");

        printf("st_size: %ld\n", stbuf.st_size);
    }

    exit(0);
}

