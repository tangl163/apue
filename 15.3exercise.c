#include "common.h"

/**
 * 15.3 What happens if the argument to popen is a nonexistent command?
 * Write a small program to test this.
 */
int
main(int argc, char *argv[])
{
    int status;
    FILE *fp;
    char buf[PIPE_BUF];

    if (argc != 2)
        err_quit("Usage: %s cmd", argv[0]);

    fp = popen(argv[1], "r");
    if (fp == NULL)
        err_sys("popen error");

    while (fgets(buf, PIPE_BUF, fp) != NULL) {
        if (fputs(buf, stdout) == EOF)
            err_sys("fputs error");
    }

    if (ferror(fp))
        err_sys("fgets error");

    status = pclose(fp);
    if (status < 0)
        err_sys("pclose error");

    pr_exit(status);

    exit(0);
}

