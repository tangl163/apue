#include "common.h"

/**
 * write what read from stdin to stderr
 */
int
main(void)
{
    char buf[BUFSIZ];

    while (fgets(buf, BUFSIZ, stdin) != NULL) {
        if (fputs(buf, stderr) == EOF)
            err_sys("fputs error");
    }

    if (ferror(stdin))
        err_sys("fgets error");

    exit(0);
}

