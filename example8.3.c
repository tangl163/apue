#include "common.h"
#include <sys/wait.h>

int globvar = 10;

int main(void)
{
    pid_t pid;
    int var, status;

    var = 11;

    if ((pid = vfork()) < 0) {
        err_sys("vfork error");

    } else if (pid == 0) {
        globvar++;
        var++;
        
        fclose(stdout);
        _exit(2);
    }

    if (wait(&status) < 0)
        err_sys("wait error");

    pr_exit(status);

    if (printf("pid: %ld globbar: %d var: %d\n", (long)getpid(), globvar, var) < 0)
        err_sys("printf error");

    exit(0);
}

