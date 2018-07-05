#include "common.h"
#include <signal.h>

static void sig_pipe(int signo);

int
main(void)
{
    int len;
    pid_t pid;
    int fd1[2], fd2[2];
    char buf[MAXLINE];

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal error");

    if (pipe(fd1) < 0)
        err_sys("pipe error");

    if (pipe(fd2) < 0)
        err_sys("pipe error");

    pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        close(fd1[1]);
        close(fd2[0]);

        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) < 0)
                err_sys("dup2 error");
            close(fd1[0]);
        }

        if (fd2[1] != STDOUT_FILENO) {
            if (dup2(fd2[1], STDOUT_FILENO) < 0)
                err_sys("dup2 error");
            close(fd2[1]);
        }

        execl("./bin/add2", "add2", (char *)0);
        err_sys("execl error");
    } else {
        close(fd1[0]);
        close(fd2[1]);

        while (fgets(buf, MAXLINE, stdin) != NULL) {
            len = strlen(buf);
            if (write(fd1[1], buf, len) != len)
                err_sys("write error");

            len = read(fd2[0], buf, MAXLINE);
            if (len < 0)
                err_sys("read error");

            buf[len] = 0;
            if (fputs(buf, stdout) == EOF)
                err_sys("fputs error");
        }

        if (ferror(stdin))
            err_sys("fgets error");
    }

    exit(0);
}

static void
sig_pipe(int signo)
{
    printf("signal caught signo: %d\n", signo);
    exit(1);
}

