#include "common.h"

int globvar = 10;

int main(void)
{
    pid_t pid;
    int var;

    var = 11;

    if ((pid = vfork()) < 0) {
        err_sys("vfork error");

    } else if (pid == 0) {
        globvar++;
        var++;
        exit(0);
    }

    printf("pid: %ld globbar: %d var: %d\n", (long)getpid(), globvar, var);

    exit(0);
}

