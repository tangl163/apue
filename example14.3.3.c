#include "common.h"

int
main(int argc, char *argv[])
{
    int fd;
    struct flock lock;

    if (argc != 2)
        err_quit("Usage: %s filename", argv[0]);

    if ((fd = open(argv[1], O_RDWR)) < 0)
        err_sys("open file: %s error", argv[1]);

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) < 0)
        err_sys("fcntl error");

    printf("File: %s has been locked\n", argv[1]);

    pause();
}

