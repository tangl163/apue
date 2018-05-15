#include "common.h"

int
main(void)
{
    DIR *dp;
    struct dirent *dirp;
    int fd, fd2, close_on_exec;

    if ((dp = opendir("/")) == NULL)
        err_sys("opendir for root error");

    while ((dirp = readdir(dp))) {

        if (strcmp(".", dirp->d_name) == 0 || strcmp("..", dirp->d_name) == 0)
            continue;

        printf("%s\n", dirp->d_name);
    }
    
    /* get file discriptor for directory stream */
    fd = dirfd(dp);

    if ((close_on_exec = fcntl(fd, F_GETFD)) == -1)
        err_sys("fcntl error");

    printf("fd: %d close on exec bit: %s\n", fd, close_on_exec ? "on" : "off");

    if ((fd2 = open("/", O_RDONLY)) < 0)
        err_sys("open error");
    
    if ((close_on_exec = fcntl(fd2, F_GETFD)) == -1)
        err_sys("fcntl error");

    printf("fd: %d close on exec bit: %s\n", fd2, close_on_exec ? "on" : "off");

    exit(0);
}

