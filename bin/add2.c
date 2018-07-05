#include "common.h"

int
main(void)
{
    int len;
    int num1, num2;
    char buf[MAXLINE];

    for ( ;; ) {
        len = read(STDIN_FILENO, buf, MAXLINE);
        if (len < 0)
            err_sys("read error");

        if (len == 0)
            break;

        if (sscanf(buf, "%d%d", &num1, &num2) != 2)
            err_quit("wrong variable\n");

        sprintf(buf, "%d\n", num1 + num2);
        len = strlen(buf);

        if (write(STDOUT_FILENO, buf, len) != len)
            err_sys("write error");
    }

    exit(0);
}

