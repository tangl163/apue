#include "common.h"

static void F_TELL_WAIT(void);
static void F_TELL_PARENT(void);
static void F_TELL_CHILD(pid_t pid);
static void F_WAIT_PARENT(void);
static void F_WAIT_CHILD(void);

static int fd;
static char *filepath;

int
main(int argc, char *argv[])
{
    pid_t pid;

    filepath = argc[0];

    F_TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        F_WAIT_PARENT();
        printf("In child\n");

        _exit(0);
    } else {
        printf("In parent\n");
        F_TELL_CHILD();
    }

    exit(0);
}

static void
F_TELL_WAIT(void)
{
    fd = open(filepath, O_RDWR);
    if (fd < 0)
        err_sys("open %s error", filepath);
}

static void
F_TELL_PARENT(void)
{
    struct flock lock;

    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_en = 0;

    if (fcntl(fd, F_SETLK, &lock) < 0)
        err_sys("F_TELL_PARENT: fcntl error");
}

static void
F_TELL_CHILD(void)
{
    struct flock lock;

    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_en = 0;

    if (fcntl(fd, F_SETLK, &lock) < 0)
        err_sys("F_TELL_PARENT: fcntl error");
}

static void
F_WAIT_PARENT(void)
{
    struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_en = 0;

    if (fcntl(fd, F_SETLKW, &lock) < 0)
        err_sys("F_TELL_PARENT: fcntl error");
}

static void
F_WAIT_CHILD(void)
{
    struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_en = 0;

    if (fcntl(fd, F_SETLKW, &lock) < 0)
        err_sys("F_TELL_PARENT: fcntl error");
}

