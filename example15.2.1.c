#include "common.h"

static void strtoupper(char *str, int len);

int
main(void)
{
    pid_t pid;
    int fd[2], n;
    char buf[PIPE_BUF];

    if (pipe(fd) < 0)
        err_sys("pipe error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        close(fd[1]);

        n = read(fd[0], buf, PIPE_BUF);
        if (n < 0)
            err_sys("read error");

        strtoupper(buf, n);

        write(STDOUT_FILENO, buf, n);
    } else {
        close(fd[0]);

        if (write(fd[1], "Hello world\n", 12) != 12)
            err_sys("write error");
    }

    exit(0);
}

static void
strtoupper(char *str, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        if (islower(str[i]))
            str[i] &= ~32;
        else if (isupper(str[i]))
            str[i] |= 32;
    }
}

