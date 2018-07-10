#include "common.h"

/**
 * 15.8 What happens if the cmdstring executed by popen with a
 * type of "r" writes to its standard error?
 */
int
main(void)
{
    int len;
    FILE *fp;
    char buf[PIPE_BUF];

    fp = popen("./bin/cat", "r");
    if (fp == NULL)
        err_sys("popen error");

    while (fgets(buf, PIPE_BUF, fp) != NULL) {
        len = strlen(buf);
        printf("%d bytes read\n", len);

        if (fputs(buf, stdout) == EOF)
            err_sys("fputs error");
    }

    if (ferror(fp))
        err_sys("fgets error");

    if (pclose(fp) < 0)
        err_sys("pclose error");

    exit(0);
}

