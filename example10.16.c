#include "common.h"

static void pr_char(char *str);

int
main(void)
{
    pid_t pid;

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");

    } else if (pid == 0) {
        pr_char("In child We set the standard output unbuffered, so every character output generates a write. "
                "The goal in this example is to allow the kernel to switch between the two processes as "
                "often as possible to demonstrate the race condition\n");
        TELL_PARENT();
        
    } else {
        WAIT_CHILD();
        pr_char("In parent We set the standard output unbuffered, so every character output generates a write. "
                "The goal in this example is to allow the kernel to switch between the two processes as "
                "often as possible to demonstrate the race condition.\n");
    }

    exit(0);
}

static void
pr_char(char *str)
{
    int c;
    char *p;

    setbuf(stdout, NULL);

    p = str;

    while ((c = *p++) != 0)
        putc(c, stdout);
}

