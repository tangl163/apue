#include "common.h"
#include <signal.h>

static void sig_pipe(int signo);

int
main(void)
{
    pid_t pid;
    FILE *fpin, *fpout;
    int fd1[2], fd2[2];
    char buf[MAXLINE];

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal error");

    if (pipe(fd1) < 0 || pipe(fd2) < 0)
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

        fpin = fdopen(fd2[0], "r");
        if (fpin == NULL)
            err_sys("fdopen for fpin error");

        fpout = fdopen(fd1[1], "w");
        if (fpout == NULL)
            err_sys("fdopen for fpout error");

        while (fgets(buf, MAXLINE, stdin) != NULL) {
            if (fputs(buf, fpout) == EOF)
                err_sys("fputs error");

            fflush(fpout);

            if (fgets(buf, MAXLINE, fpin) == NULL)
                err_sys("read error");

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

