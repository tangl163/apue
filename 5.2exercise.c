#include "common.h"

/**
 * 5.2 Type in the program that copies a ﬁle using line-at-a-time I/O (fgets and fputs)from
 * Figure5.5, but use a MAXLINE of 4. What happens if you copy lines that exceed this length?
 * Explain what is happening.
 */
int
main(void)
{
    char buf[4];

    while (fgets(buf, 4, stdin) != NULL)
        if (fputs(buf, stdout) == EOF)
            err_sys("output error");

    if (ferror(stdin))
        err_sys("input error");

    exit(0);
}

