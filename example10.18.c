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
    struct sigaction sig_int, sig_int_old, sig_quit, sig_quit_old;

    if (cmdstring == NULL)
        return 1;

    sig_int.sa_flags = 0;
    sig_quit.sa_flags = 0;
    sig_int.sa_handler = SIG_IGN;
    sig_quit.sa_handler = SIG_IGN;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigemptyset(&sig_int.sa_mask);
    sigemptyset(&sig_quit.sa_mask);

    sigaction(SIGINT, &sig_int, &sig_int_old);
    sigaction(SIGQUIT, &sig_quit, &sig_quit_old);
    sigprocmask(SIG_SETMASK, &mask, &omask);

    if ((pid = fork()) < 0) {
        status = -1;

    } else if (pid == 0) {
        sigaction(SIGINT, &sig_int_old, NULL);
        sigaction(SIGQUIT, &sig_quit_old, NULL);
        sigprocmask(SIG_SETMASK, &omask, NULL);

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);
    }
    
    if (waitpid(pid, &status, 0) < 0)
        status = -1;
    
    sigaction(SIGINT, &sig_int_old, NULL);
    sigaction(SIGQUIT, &sig_quit_old, NULL);
    sigprocmask(SIG_SETMASK, &omask, NULL);

    return status;
}

