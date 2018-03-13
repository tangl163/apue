#include "common.h"

#define TEST_FILE "/tmp/test_file"

static int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

int
main(void)
{
    int fd;
    pid_t pid;

    if ((fd = open(TEST_FILE, O_WRONLY)) < 0)
        err_sys("open error");

    if (lock_reg(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 1) < 0)
        err_sys("Parent: lock_reg error");

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (lock_reg(fd, F_SETLK, F_WRLCK, 1, SEEK_SET, 1) < 0)
            err_sys("Child: lock_reg error");

        TELL_PARENT();

        if (lock_reg(fd, F_SETLKW, F_WRLCK, 0, SEEK_SET, 1) < 0)
            err_sys("Child: lock_reg error");

    } else {
        WAIT_CHILD();
        if (lock_reg(fd, F_SETLKW, F_WRLCK, 1, SEEK_SET, 1) < 0)
            err_sys("Parent: lock_reg error");
    }

    exit(0);
}

static int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type = type;
    lock.l_whence = whence;
    lock.l_start = offset;
    lock.l_len = len;

    return fcntl(fd, cmd, &lock);
}

