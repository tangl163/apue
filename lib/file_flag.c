#include "common.h"

/**
 * Set the file descriptor flag of fd to flag
 */
void set_flag(int fd, int flag)
{
    int val;

    if ((val = fcntl(fd, F_GETFD)) < 0)
        err_sys("fcntl F_GETFD error");

    val |= flag;

    if (fcntl(fd, F_SETFD, val) < 0)
        err_sys("fcntl F_SETFD error");
}

/**
 * Clear the file descriptor flag of fd with flag
 */
void clear_flag(int fd, int flag)
{
    int val;

    if ((val = fcntl(fd, F_GETFD)) < 0)
        err_sys("fcntl F_GETFD error");

    val &= ~flag;

    if (fcntl(fd, F_SETFD, val) < 0)
        err_sys("fcntl F_SETFD error");
}

