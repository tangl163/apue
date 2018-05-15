#include "common.h"

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int
main(int argc, char *argv[])
{
    int fd;

    unlink(argv[1]);

    if ((fd = creat(argv[1], RWRWRW)) < 0)
        err_sys("creat error for: %s", argv[1]);

    if (write(fd, "nihao", 5) != 5)
        err_sys("write error");

    close(fd);

    exit(0);
}

