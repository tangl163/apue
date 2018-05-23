#include "common.h"
#include <sys/mman.h>
#include <signal.h>

int
main(int argc, char *argv[])
{
    int fd;
    unsigned char *addr;

    if (argc != 2)
        err_quit("Usage: %s filename", argv[0]);

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
        err_sys("open error: %s", argv[1]);

    addr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        err_sys("mmap error");

    sleep(10);

    addr[0] = 'a';

    munmap(addr, 1024);
    close(fd);

    exit(0);
}

