#include "common.h"

static void P_TELL_WAIT(void);
static void P_TELL_PARENT(void);
static void P_TELL_CHILD(void);
static void P_WAIT_PARENT(void);
static void P_WAIT_CHILD(void);

static int fd1[2], fd2[2];

int
main(void)
{
    pid_t pid;

    P_TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        P_WAIT_PARENT();
        printf("In child\n");
        P_TELL_PARENT();

        _exit(0);
    } else {
        printf("In parent\n");
        P_TELL_CHILD();
        P_WAIT_CHILD();
    }

    exit(0);
}

static void
P_TELL_WAIT(void)
{
    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        err_sys("pipe error");
}

static void
P_TELL_PARENT(void)
{
    if (write(fd2[1], "c", 1) != 1)
        err_sys("write error");
}

static void
P_TELL_CHILD(void)
{
    if (write(fd1[1], "p", 1) != 1)
        err_sys("write error");
}

static void
P_WAIT_PARENT(void)
{
    char c;

    if (read(fd1[0], &c, 1) < 0)
        err_sys("read error");

    if (c != 'p')
        err_quit("P_WAIT_PARENT: incorrect data");
}

static void
P_WAIT_CHILD(void)
{
    char c;

    if (read(fd2[0], &c, 1) < 0)
        err_sys("read error");

    if (c != 'c')
        err_quit("P_WAIT_CHILD: incorrect data");
}

