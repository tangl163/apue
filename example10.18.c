#include "common.h"
#include <signal.h>
#include <sys/wait.h>

static int mysystem(const char *cmdstring);

int
main(int argc, char *argv[])
{
    int status;

    if (argc != 2)
        err_quit("Usage: %s cmd", argv[0]);

    if ((status = mysystem(argv[1])) < 0)
        err_sys("mysystem error");

    pr_exit(status);

    exit(0);
}

static int
mysystem(const char *cmdstring)
{
    int status;
    pid_t pid;
    sigset_t mask, omask;
    struct sigaction ignore, sig_int_old, sig_quit_old;

    if (cmdstring == NULL)
        return 1;

    ignore.sa_flags = 0;
    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

    if (sigaction(SIGINT, &ignore, &sig_int_old)) {
        status = -1;
        goto result;
    }

    if (sigaction(SIGQUIT, &ignore, &sig_quit_old)) {
        status = -1;
        goto result;
    }

    if (sigprocmask(SIG_BLOCK, &mask, &omask)) {
        status = -1;
        goto result;
    }

    if ((pid = fork()) < 0) {
        status = -1;

    } else if (pid == 0) {
        sigaction(SIGINT, &sig_int_old, NULL);
        sigaction(SIGQUIT, &sig_quit_old, NULL);
        sigprocmask(SIG_SETMASK, &omask, NULL);

        /*
         * We use /bin/bash instead of /bin/sh. In my ubuntu server, /bin/sh
         * is a link pointed to /bin/dash, which won't handle SIGQUIT.
         */
        execl("/bin/bash", "sh", "-c", cmdstring, (char *)0);
        _exit(127);
    }
    
    if (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR) {
            errno = 0;
            status = -1;
        }
    }
    
    if (sigaction(SIGINT, &ignore, NULL) < 0) {
        status = -1;
        goto result;
    }

    if (sigaction(SIGQUIT, &ignore, NULL)) {
        status = -1;
        goto result;
    }

    if (sigprocmask(SIG_SETMASK, &omask, NULL))
        status = -1;

result:

    return status;
}

