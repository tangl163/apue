#include "common.h"

#define FIFO_FILE "/tmp/tmp_fifo"

int
main(void)
{
    int n, fd;
    char buf[PIPE_BUF];

    fd = open(FIFO_FILE, O_RDONLY | O_CREAT | O_NONBLOCK);
    if (fd < 0)
        err_sys("open error");

    n = read(fd, buf, PIPE_BUF);
    if (n < 0)
        err_sys("read error\n");

    printf("%d bytes read\n", n);

    exit(0);
}

