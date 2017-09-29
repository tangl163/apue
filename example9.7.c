#include "common.h"
#include <termios.h>

int
main(void)
{
    int fd;
    pid_t pid;

    if ((fd = open("/dev/tty", O_RDWR)) == -1)
        err_sys("open /dev/tty failure");

    if ((pid = tcgetsid(fd)) < 0)
        err_sys("tcgetsid error");

    printf("sid: %d\n", pid);

    exit(0);
}

