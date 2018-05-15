#include "common.h"
#include <sys/resource.h>

int
main(void)
{
    struct rlimit rlptr;

    if (getrlimit(RLIMIT_NOFILE, &rlptr) < 0)
        err_sys("getrlimit error");

    printf("soft limit: %ld\n", rlptr.rlim_cur);
    printf("hard limit: %ld\n", rlptr.rlim_max);

    exit(0);
}

