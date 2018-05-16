#include "common.h"
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int fd;
    pid_t pid;
    struct flock lock;

    if (argc != 2)
        err_quit("Usage: %s filepath", argv[1]);

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
        err_sys("open error: %s", argv[1]);

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 10;

    /* read lock the first 10 bytes */
    if (fcntl(fd, F_SETLK, &lock) < 0)
        err_sys("fcntl error");

    printf("Parent: read lock success\n");

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 5;
    lock.l_len = 10;

    /* write lock 5~15 range */
    if (fcntl(fd, F_SETLK, &lock) < 0)
        err_sys("fcntl error");

    printf("Parent: write lock success\n");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {

        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 5;
        lock.l_len = 5;

        /* get lock 5~10 range */
        if (fcntl(fd, F_GETLK, &lock) < 0)
            err_sys("fcntl error");

        if (lock.l_type != F_UNLCK)
            printf("Region has been locked by process: %ld\n", (long)lock.l_pid);
        else
            printf("Child: get lock success\n");

        printf("l_type: %d\n", lock.l_type);
        printf("l_start: %ld\n", lock.l_start);
        printf("l_len: %ld\n", lock.l_len);
        printf("l_pid: %ld\n", (long)lock.l_pid);
        _exit(0);
    }

    wait(NULL);

    exit(0);
}

