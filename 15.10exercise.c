#include "common.h"

#define TMP_FIFO "/tmp/tmp_fifo"

/**
 * 15.10 POSIX.1 specifically states that opening a FIFO for read–write
 * is undefined. Although most UNIX systems allow this, show another
 * method for opening a FIFO for both reading and writing, without blocking.
 */
int
main(void)
{
    int len, fd;
    char buf[PIPE_BUF];

    if (mkfifo(TMP_FIFO, S_IRUSR | S_IWUSR) < 0)
        err_sys("mkfifo error");

    fd = open(TMP_FIFO, O_RDWR | O_NONBLOCK);
    if (fd < 0)
        err_sys("open error");

    while (fgets(buf, PIPE_BUF, stdin) != NULL) {
        len = strlen(buf);

        if (write(fd, buf, len) != len)
            err_sys("write to pipe error");

        len = read(fd, buf, PIPE_BUF);
        if (len < 0)
            err_sys("read from pipe error");

        if (len == 0) {
            fprintf(stderr, "pipe closed\n");
            break;
        }

        if (write(STDOUT_FILENO, buf, len) != len)
            err_sys("write error");
    }

    if (ferror(stdin))
        err_sys("fgets error");

    unlink(TMP_FIFO);

    exit(0);
}

