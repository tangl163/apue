#include "common.h"

#define BUFSIZE 5000000

int
main(void)
{
    int nread, nwrite;
    char buf[BUFSIZE];
    char *ptr;

    set_fl(STDOUT_FILENO, O_NONBLOCK);

    nread = read(STDIN_FILENO, buf, BUFSIZE);
    if (nread < 0)
        err_sys("read error");

    ptr = buf;

    while (nread > 0) {
        nwrite = write(STDOUT_FILENO, ptr, nread);
        fprintf(stderr, "nwrite: %d. errno: %d\n", nwrite, errno);

        if (nwrite) {
            nread -= nwrite;
            ptr += nwrite;
        }
    }

    clear_fl(STDOUT_FILENO, O_NONBLOCK);

    exit(0);
}

