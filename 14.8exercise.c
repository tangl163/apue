#include "common.h"
#include <aio.h>

static void rot13_translate(char *str, long len);

int
main(void)
{
    off_t r_offset, w_offset;
    int nread, nwrite, err;
    char buf[BUFSIZ];
    struct aiocb aiobuf[2];
    const struct aiocb *listin[1];
    const struct aiocb *listout[1];

    r_offset = w_offset = 0;
    listin[0] = &aiobuf[0];
    listout[0] = &aiobuf[1];

    /* index 0 for reading */
    aiobuf[0].aio_fildes = STDIN_FILENO;
    aiobuf[0].aio_buf = buf;
    aiobuf[0].aio_nbytes = BUFSIZ;
    aiobuf[0].aio_sigevent.sigev_notify = SIGEV_NONE;

    /* index 1 for writing */
    aiobuf[1].aio_fildes = STDOUT_FILENO;
    aiobuf[1].aio_buf = buf;
    aiobuf[1].aio_sigevent.sigev_notify = SIGEV_NONE;

    for ( ;; ) {
        aiobuf[0].aio_offset = r_offset;
        aiobuf[1].aio_offset = w_offset;

        if (aio_read(&aiobuf[0]) == -1)
            err_sys("aio_read error");

        if (aio_suspend(listin, 1, NULL) == -1)
            err_sys("aio_suspend error");

        err = aio_error(&aiobuf[0]);
        if (err != 0) {
            if (err == -1)
                err_sys("aio_error error");
            else
                err_exit(err, "read error");
        }

        nread = aio_return(&aiobuf[0]);
        if (nread == -1)
            err_sys("aio_return error");

        if (nread == 0)
            break;

        r_offset += nread;

        rot13_translate(buf, nread);

        if (aio_write(&aiobuf[1]) == -1)
            err_sys("aio_write error");

        if (aio_suspend(listout, 1, NULL) == -1)
            err_sys("aio_suspend error");

        err = aio_error(&aiobuf[1]);
        if (err != 0) {
            if (err == -1)
                err_sys("aio_error error");
            else
                err_exit(err, "write error");
        }

        nwrite = aio_return(&aiobuf[1]);
        if (nwrite == -1)
            err_sys("aio_return error");

        //if (nwrite != aiobuf[1].aio_nbytes)
        //    err_quit("short write (%d/%d)", nwrite, aiobuf[1].aio_nbytes);

        w_offset += nwrite;
    }

    exit(0);
}

static void
rot13_translate(char *str, long len)
{
    while (len--) {
        if (isalpha(*str)) {
            if (*str >= 'n')
                *str -= 13;
            else if (*str >= 'a')
                *str += 13;
            else if (*str >= 'N')
                *str -= 13;
            else
                *str += 13;
        }
        str++;
    }
}

