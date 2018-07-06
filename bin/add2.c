#include "common.h"

int
main(void)
{
    int len;
    int num1, num2;
    char buf[MAXLINE];

    while ((len = read(STDIN_FILENO, buf, MAXLINE)) > 0) {
        if (sscanf(buf, "%d%d", &num1, &num2) != 2) {
            write(STDOUT_FILENO, "invalid args\n", 13);
            continue;
        }

        sprintf(buf, "%d\n", num1 + num2);
        len = strlen(buf);

        write(STDOUT_FILENO, buf, len);
    }

    exit(0);
}

