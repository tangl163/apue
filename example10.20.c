#include "common.h"
#include <signal.h>

int
main(void)
{
    long n;

    if ((n = sysconf(_SC_SIGQUEUE_MAX)) < 0 && errno != 0)
        err_sys("sysconf error");

    printf("sig_queue_max: %ld\n", n);

    exit(0);
}

