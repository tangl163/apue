#include "common.h"

static pid_t callvfork(void);

/**
 * 8.2 Recall the typical arrangement of memory in Figure 7.6.
 * Because the stack frames corresponding to each function call
 * are usually stored in the stack, and because after a vfork
 * the child runs in the address space of the parent,
 * what happens if the call to vfork is from a function other
 * than main and the child does a return from this function after
 * the vfork? Write a test program to verify this, and draw a picture of what’s happening.
 */
int
main(void)
{
    pid_t pid;

    pid = callvfork();

    printf("pid=%d\n", pid);

    exit(0);
}

static pid_t
callvfork(void)
{
    pid_t pid;

    if ((pid = vfork()) < 0) {
        err_sys("vfork error");
    } else if (pid == 0) {
        return getpid();
    }

    return getpid();
}

