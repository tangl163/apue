#include "common.h"
#include <time.h>
#include <sys/time.h>

/**
 * 4.13 How can you set only one of the two time values with the utimes function?
 */
int
main(int argc, char *argv[])
{
    time_t now;
    struct stat statbuf;
    struct timeval times[2];

    if (argc != 2)
        err_quit("Usage: %s <filename>", argv[0]);

    now = time(NULL);   

    if (lstat(argv[1], &statbuf) < 0)
        err_sys("%s: lstat error", argv[1]);

    /* we just change the access time. */
    times[0].tv_sec = now;
    times[0].tv_usec = 0;

    /* we leave modification time unchanged */
    times[1].tv_sec = statbuf.st_mtim.tv_sec;
    times[1].tv_usec = 0;

    if (utimes(argv[1], times) < 0)
        err_sys("utimes for %s error", argv[1]);

    exit(0);
}

