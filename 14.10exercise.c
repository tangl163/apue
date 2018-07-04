#include "common.h"
#include <sys/mman.h>

#define COPY_SIZE (1024 * 1024)

/**
 * 14.10 Run the program in Figure 14.27 to copy a ﬁle and determine
 * whether the last-access time for the input ﬁle is updated.
 */
int
main(int argc, char *argv[])
{
    off_t offset;
    size_t len;
    int infd, outfd;
    char *inaddr, *outaddr;
    struct stat statbuf;

    if (argc != 3)
        err_quit("Usage: %s inputfile outputfile", argv[0]);

    if (stat(argv[1], &statbuf) < 0)
        err_sys("stat error");

    printf("before copying:\n");
    printf("seconds: %ld\n", statbuf.st_atim.tv_sec);
    printf("nanoseconds: %ld\n", statbuf.st_atim.tv_nsec);

    infd = open(argv[1], O_RDONLY);
    if (infd < 0)
        err_sys("open error");

    outfd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (outfd < 0)
        err_sys("open error");

    if (ftruncate(outfd, statbuf.st_size) < 0)
        err_sys("ftruncate error");

    if (statbuf.st_size > COPY_SIZE)
        len = COPY_SIZE;
    else
        len = statbuf.st_size;

    offset = 0;

    while (offset < statbuf.st_size) {
        inaddr = mmap(NULL, len, PROT_READ, MAP_SHARED, infd, offset);
        if (inaddr == MAP_FAILED)
            err_sys("mmap error");

        outaddr = mmap(NULL, len, PROT_WRITE, MAP_SHARED, outfd, offset);
        if (outaddr == MAP_FAILED)
            err_sys("mmap error");

        memcpy(outaddr, inaddr, len);
        munmap(inaddr, len);
        munmap(outaddr, len);
        offset += len;
    }

    if (fstat(outfd, &statbuf) < 0)
        err_sys("fstat error");

    printf("after copying:\n");
    printf("seconds: %ld\n", statbuf.st_atim.tv_sec);
    printf("nanoseconds: %ld\n", statbuf.st_atim.tv_nsec);

    close(infd);
    close(outfd);

    exit(0);
}

