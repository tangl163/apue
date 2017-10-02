#include "common.h"
#include <termios.h>

static void check_ct(char *s);

/**
 * 9.2 Write a small program that calls fork and has the child create a new session.
 * Verify that the child becomes a process group leader and that the child no longer has a controlling terminal.
 */
int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        setsid();
        printf("Is process group leader: %s\n", getpid() == getpgrp() ? "yes" : "no");
        check_ct("child");

    } else {
        check_ct("parent");
        _exit(0);
    }

    exit(0);
}

static void
check_ct(char *s)
{
    int fd;

    if ((fd = open("/dev/tty", O_RDWR)) == -1)
        printf("%s: Has controlling terminal? no\n", s);
    else
        printf("%s: Has controlling terminal? yes\n", s);
}

