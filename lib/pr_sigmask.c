#include "../common.h"
#include <signal.h>

void
pr_sigmask(const char *str)
{
    sigset_t oldset;
    int errno_save;

    errno_save = errno;

    if (sigprocmask(0, NULL, &oldset) < 0) {
        err_ret("pr_sigmask: sigpromask error");

    } else {
        printf("%s:\n", str);

        if (sigismember(&oldset, SIGHUP))
            printf("SIGHUP ");

        if (sigismember(&oldset, SIGINT))
            printf("SIGINT ");

        if (sigismember(&oldset, SIGQUIT))
            printf("SIGQUIT ");

        if (sigismember(&oldset, SIGILL))
            printf("SIGILL ");

        if (sigismember(&oldset, SIGTRAP))
            printf("SIGTRAP ");

        if (sigismember(&oldset, SIGABRT))
            printf("SIGABRT ");

        if (sigismember(&oldset, SIGIOT))
            printf("SIGIOT ");

        if (sigismember(&oldset, SIGBUS))
            printf("SIGBUS ");

        if (sigismember(&oldset, SIGFPE))
            printf("SIGFPE ");

        if (sigismember(&oldset, SIGKILL))
            printf("SIGKILL ");

        if (sigismember(&oldset, SIGUSR1))
            printf("SIGUSR1 ");

        if (sigismember(&oldset, SIGSEGV))
            printf("SIGSEGV ");

        if (sigismember(&oldset, SIGUSR2))
            printf("SIGUSR2 ");

        if (sigismember(&oldset, SIGPIPE))
            printf("SIGPIPE ");

        if (sigismember(&oldset, SIGALRM))
            printf("SIGALRM ");

        if (sigismember(&oldset, SIGTERM))
            printf("SIGTERM ");

        if (sigismember(&oldset, SIGSTKFLT))
            printf("SIGSTKFLT ");

        if (sigismember(&oldset, SIGCHLD))
            printf("SIGCHLD ");

        if (sigismember(&oldset, SIGCONT))
            printf("SIGCONT ");

        if (sigismember(&oldset, SIGSTOP))
            printf("SIGSTOP ");

        if (sigismember(&oldset, SIGTSTP))
            printf("SIGTSTP ");

        if (sigismember(&oldset, SIGTTIN))
            printf("SIGTTIN ");

        if (sigismember(&oldset, SIGTTOU))
            printf("SIGTTOU ");

        if (sigismember(&oldset, SIGURG))
            printf("SIGURG ");

        if (sigismember(&oldset, SIGXCPU))
            printf("SIGXCPU ");

        if (sigismember(&oldset, SIGXFSZ))
            printf("SIGXFSZ ");

        if (sigismember(&oldset, SIGVTALRM))
            printf("SIGVTALRM ");

        if (sigismember(&oldset, SIGPROF))
            printf("SIGPROF ");

        if (sigismember(&oldset, SIGWINCH))
            printf("SIGWINCH ");

        if (sigismember(&oldset, SIGIO))
            printf("SIGIO ");

        if (sigismember(&oldset, SIGPWR))
            printf("SIGPWR ");

        if (sigismember(&oldset, SIGSYS))
            printf("SIGSYS ");
    }

    printf("\n");

    errno = errno_save;
}

