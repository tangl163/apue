#include "common.h"
#include <time.h>
#include <sys/time.h>

/**
 * 10.10 Write a program that calls sleep(60) in an infinite loop. Every five
 * times through the loop (every 5 minutes), fetch the current time of day and
 * print the tm_sec field. Run the program overnight and explain the results.
 * How would a program such as the cron daemon, which runs every minute on the
 * minute, handle this situation?
 */

int
main(void)
{
    int count = 0;
    struct timespec tsp;

    while (TRUE) {
        sleep(60);
        count++;

        if (count == 5) {
            if (clock_gettime(CLOCK_REALTIME, &tsp) < 0)
                err_sys("clock_gettime error");

            printf("seconds: %ld, nanoseconds: %ld\n", tsp.tv_sec, tsp.tv_nsec);
            count = 0;
        }
    }

    exit(0);
}

