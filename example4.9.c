#include "common.h"

#define RWRWRW  (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(void)
{
    umask(0);

    if (creat("foo", RWRWRW) < 0)
        err_sys("creat foo error");

    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (creat("bar", RWRWRW) < 0)
        err_sys("creat bar error");

    exit(0);
}

