#include "common.h"

/**
 * 16.1 Write a program to determine your system’s byte ordering.
 */
int
main(void)
{
    int i = 1;

    if (*(char *)&i == 1)
        printf("Little endian\n");
    else
        printf("Big endian\n");

    exit(0);
}

