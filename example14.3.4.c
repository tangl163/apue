#include "common.h"
#include <sys/wait.h>

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH

int
main(int argc, char *argv[])
{
    int fd;
    pid_t pid;
    char buf[BUFSIZ];
    struct flock lock;
    struct stat statbuf;

    if (argc != 2)
        err_quit("Usage: %s filename", argv[0]);

    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
        err_sys("open file: %s error", argv[1]);

    if (write(fd, "hello", 5) != 5)
        err_sys("write to fd: %d error", fd);

    if (fstat(fd, &statbuf) < 0)
        err_sys("fstat error");

    statbuf.st_mode |= S_ISGID;
    statbuf.st_mode &= ~S_IXGRP;

    if (fchmod(fd, statbuf.st_mode) < 0)
        err_sys("fchmod error");

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) < 0)
        err_sys("fcntl error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        set_fl(fd, O_NONBLOCK);

        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;

        if (fcntl(fd, F_SETLK, &lock) < 0)
            printf("read lock fail: %s\n", strerror(errno));
        else
            printf("read lock on already-locked region success\n");

        if (lseek(fd, 0, SEEK_SET) < 0)
            err_sys("lseek error");

        if (read(fd, buf, BUFSIZ) < 0)
            printf("read error. Mandatory locking works\n");
        else
            printf("read OK. No mandatory locking\n");

        _exit(0);
    }

    if (waitpid(pid, NULL, 0) < 0)
        err_sys("wait error");

    exit(0);
}

