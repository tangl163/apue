#include "common.h"
#include <sys/uio.h>
#include <sys/times.h>

#define INIT_SIZE (1024 * 1024)
#define SIZE_INCREMENT 4096

/**
 * 14.9 Recall Figure14.23. Determine the break-even point on your system
 * where using writev is faster than copying the data yourself and using
 * a single write.
 */
int
main(void)
{
    int fd;
    size_t size;
    clock_t consume1, consume2;
    char *buf1, *buf2, *buf3;
    struct iovec vec[2];
    struct tms start, end;

    size = INIT_SIZE;

    fd = open("/tmp/tmp.out", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0)
        err_sys("open error");

    buf1 = malloc(sizeof(char) * size);
    if (buf1 == NULL)
        err_sys("malloc error");

    buf2 = malloc(sizeof(char) * size);
    if (buf2 == NULL)
        err_sys("malloc error");

    buf3 = malloc(sizeof(char) * size * 2);
    if (buf3 == NULL)
        err_sys("malloc error");

    memset(buf1, 'a', size);
    memset(buf2, 'a', size);
    memset(buf3, 'a', size * 2);

    for ( ;; ) {
        vec[0].iov_base = buf1;
        vec[0].iov_len = size;
        vec[1].iov_base = buf2;
        vec[1].iov_len = size;

        if (times(&start) == -1)
            err_sys("times error");

        if (writev(fd, vec, 2) == -1)
            err_sys("writev error");

        if (times(&end) == -1)
            err_sys("times error");

        consume1 = end.tms_utime - start.tms_utime
            + end.tms_stime - start.tms_stime;

        if (times(&start) == -1)
            err_sys("times error");

        memcpy(buf3, buf1, size);
        memcpy(buf3+size, buf2, size);

        if (write(fd, buf3, size * 2) == -1)
            err_sys("write error");

        if (times(&end) == -1)
            err_sys("times error");

        consume2 = end.tms_utime - start.tms_utime
            + end.tms_stime - start.tms_stime;

        if (consume1 != 0 && consume2 != 0 && consume2 > consume1)
            break;

        size += SIZE_INCREMENT;
        buf1 = realloc(buf1, size);
        if (buf1 == NULL)
            err_sys("realloc error");

        buf2 = realloc(buf2, size);
        if (buf2 == NULL)
            err_sys("realloc error");

        buf3 = realloc(buf3, size * 2);
        if (buf3 == NULL)
            err_sys("realloc error");
    }

    free(buf1);
    free(buf2);
    free(buf3);

    printf("size: %ld\n", size);

    exit(0);
}

