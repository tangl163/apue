#include "common.h"

#define TEST_FILE "/tmp/test_file"
#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH

static int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

int
main(void)
{
    int fd;
    pid_t pid;

    fd = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE);
    if (fd < 0)
        err_sys("open file: %s error", TEST_FILE);

    if (write(fd, "hello", 5) != 5)
        err_sys("write error");

    /* we set read lock on byte 1 */
    if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 1) < 0)
        err_sys("set F_RDLCK error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        /*
         * Then we set write lock on byte 1. We use F_SETLKW so that
         * we would be blocked if the lock could not be acquired.
         */
        if (lock_reg(fd, F_SETLKW, F_WRLCK, 0, SEEK_SET, 1) < 0)
            err_sys("First child: set F_WRLCK error");
        printf("First child: got the lock\n");
        _exit(0);
    }

    sleep(1);    // Sleep to let the child run first. (inperfect way)
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        /* In the second child, we try to acquire read lock on byte 1 */
        if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 1) < 0) {
            if (errno == EAGAIN)
                printf("Second child: couldn't get the lock\n");
            else
                err_sys("fcntl error");
        } else {
            printf("Second child: got the lock\n");
        }

        _exit(0);
    }

    sleep(3);

    printf("Main process is about to exit\n");
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

