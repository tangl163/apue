#include "common.h"

int
main(int argc, char *argv[])
{
    FILE *fp1, *fp2;
    char buf[PIPE_BUF];

    if (argc != 2)
        err_quit("Usage: %s <filepath>", argv[0]);

    fp1 = fopen(argv[1], "r");
    if (fp1 == NULL)
        err_sys("fopen for %s error", argv[1]);

    fp2 = popen("less", "w");
    if (fp2 == NULL)
        err_sys("popen error");

    while (fgets(buf, PIPE_BUF, fp1) != NULL)
        if (fputs(buf, fp2) == EOF)
            err_sys("fputs error");

    if (ferror(fp1))
        err_sys("fgets error");

    if (pclose(fp2) < 0)
        err_sys("pclose error");

    fclose(fp1);

    exit(0);
}

