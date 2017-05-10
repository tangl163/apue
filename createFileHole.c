#include "common.h"

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(int argc, char *argv[])
{
    int fd, i;
    char *buf = "Hello world!";

    if (argc == 1)
        err_quit("Usage: %s offset1 offset2 ...", argv[0]);

    if ((fd = open("file.hole", O_RDWR | O_CREAT | O_TRUNC, RWRWRW)) < 0)
        err_sys("open file error");
    
    for (i = 1; i < argc; i++) {
        if (write(fd, buf, 12) != 12)
            err_sys("write error");

        if (lseek(fd, atoi(argv[i]), SEEK_CUR) == -1)
            err_sys("lseek error");
    }

    exit(0);
}

