#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/resource.h>

#define OPEN_MAX_GUESS 256;

long get_openmax()
{
    long openmax;
    struct rlimit rl;

    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 || openmax == LONG_MAX) {
        if (getrlimit(RLIMIT_NOFILE, &rl) != 0) {
            printf("getrlimit error: %s\n", strerror(errno));
            exit(0);
        }

        if (rl.rlim_max == RLIM_INFINITY) {
            openmax = OPEN_MAX_GUESS;
        } else {
            openmax = rl.rlim_max;
        }
    }

    return openmax;
}

void main(void)
{
    //printf("%ld\n", sysconf(_SC_OPEN_MAX));
    //printf("%ld\n", LONG_MAX);
    printf("%ld\n", get_openmax()); 
}

