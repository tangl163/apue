#include "common.h"

/**
 * 8.6 Write a program that creates a zombie, and then call system
 * to execute the ps(1) command to verify that the process is a zombie.
 */
int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        exit(0);
    }

    sleep(2);

    system("ps");

    exit(0);
}

