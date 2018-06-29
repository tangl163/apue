#include "common.h"
#include <sys/mman.h>

static long update(long *arg);

int
main(int argc, char *argv[])
{
    long n;
    int i, fd;
    pid_t pid;
    void *addr;

    if (argc != 2)
        err_quit("Usage: %s pathname", argv[0]);

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
        err_sys("open error");

    addr = mmap(0, sizeof(long), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        err_sys("mmap error");

    TELL_WAIT();

    pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        for (i = 0; i < 1000; i += 2) {
            n = update((long *)addr);
            if (n != i)
                err_quit("expect: %d actual: %ld\n", i, n);

            TELL_PARENT();
            WAIT_PARENT();
        }
    } else {
        for (i = 1; i < 1000; i += 2) {
            WAIT_CHILD();

            n = update((long *)addr);
            if (n != i)
                err_quit("expect: %d actual: %ld\n", i, n);

            TELL_CHILD(pid);
        }
    }

    exit(0);
}

static long
update(long *arg)
{
    return (*arg)++;
}

