#include "common.h"

int main(int argc, char *argv[])
{
    int fd;
    struct stat st;
    struct timespec times[2];

    if (argc != 2)
        err_quit("Usage: %s <filename>", argv[0]);

    if (stat(argv[1], &st) < 0) 
        err_sys("stat error");

    if ((fd = open(argv[1], O_WRONLY | O_TRUNC)) < 0)
        err_sys("open file %s error", argv[1]);

    /* reset access time */
    times[0] = st.st_atim;

    /* reset modification time */
    times[1] = st.st_mtim;

    if (futimens(fd, times) < 0)
        err_sys("futimesn error");

    exit(0);
}

