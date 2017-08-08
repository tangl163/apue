#include "common.h"
#include <time.h>

#define MAXSIZE 128

/**
 * 6.5 Write a program to obtain the current time and print it using
 * strftime, so that it looks like the default output from date(1).
 * Set the TZ environment variable to different values and see what happens.
 */
int main(void)
{
    time_t t;
    struct tm *tp;
    char buf[MAXSIZE]; 

    time(&t);

    if ((tp = localtime(&t)) == NULL)
        err_sys("call localtime error");  

    if (strftime(buf, MAXSIZE, "%a %b %e %T %Z %Y", tp) == 0)
        err_sys("strftime error");

    printf("%s\n", buf);

    exit(0);
}

