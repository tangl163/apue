#include "common.h"
#include <signal.h>
#include <sys/wait.h>

static int mysystem(const char *cmdstring);

int
main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s cmd", argv[0]);


}

static int
mysystem(const char *cmdstring)
{
    int status;
    pid_t pid;
    sigset_t block;

    if ((pid = fork()) < 0) {
        status = -1;

    } else if (pid == 0) {
        
    }
}

