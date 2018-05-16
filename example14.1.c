#include "common.h"

#define BUFSIZE 5000000

int
main(void)
{
    int fd, nread, nwrite;
    char buf[BUFSIZE];
    char *ptr;

    fd = open("/dev/fd/1", O_WRONLY | O_NONBLOCK);
    if (fd < 0)
        err_sys("open for /dev/fd/1 error");

    nread = read(STDIN_FILENO, buf, BUFSIZE);
    if (nread < 0)
        err_sys("read error");

    ptr = buf;

    while (nread > 0) {
        errno = 0;
        nwrite = write(fd, ptr, nread);
        fprintf(stderr, "nwrite: %d. errno: %d\n", nwrite, errno);

        if (nwrite > 0) {
            nread -= nwrite;
            ptr += nwrite;
        }
    }

    exit(0);
}

