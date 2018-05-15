#include "common.h"

/* a local implemention of dup2 */

static int local_dup2(int fd, int fd2);

extern long open_max(void);

int
main(void)
{
    int fd;

    fd = local_dup2(STDOUT_FILENO, 10);

    write(STDOUT_FILENO, "haha\n", 5);
    write(fd, "\nhaha", 5);
    printf("\n%d\n", fd);

    exit(0);
}


static int
local_dup2(int fd, int fd2)
{
    long max_fileno;
    int temp, i = 0;
    int *closed;

    if (fd == fd2)   
        return fd;

    max_fileno = open_max();

    if (fd2 >= max_fileno)
        err_quit("fd2 too big to dup");

    if ((closed = malloc(sizeof *closed * max_fileno)) == NULL)
        err_sys("Call malloc error");

    do {
        if ((temp = dup(fd)) < 0)
            err_sys("dup error");
    
        if (temp < fd2) {
            closed[i++] = temp;
        } else if (temp > fd2) {
            close(fd2);
        }

    } while (temp != fd2);

    while (i >= 0)
        close(closed[i--]);
    
    free(closed);
    
    return fd2;
}

