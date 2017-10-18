#include "common.h"
#include <signal.h>

#define MY_ABRT  1
#define SYS_ABRT 2

static void myabort(void);
static void sig_abort(int);

int
main(int argc, char *argv[])
{
    int call_type;

    if (argc != 2)
        err_quit("Usage: %s 1|2 (1: myabort; 2: abort)", argv[0]);

    call_type = atoi(argv[1]);

    if (signal(SIGABRT, sig_abort) == SIG_ERR)
        err_sys("Install signal for SIGABRT error");

    if (call_type == MY_ABRT)
        myabort();
    else if (call_type == SYS_ABRT)
        abort();
    else
        raise(SIGABRT);

    exit(0);
}

static void
myabort(void)
{
    struct sigaction ac;
    sigset_t mask;

    sigfillset(&mask);
    sigdelset(&mask, SIGABRT);

    /* We first block all signals other than SIGABRT. */
    if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0)
        err_sys("sigprocmask error");

    /* Fetch the old disposition. */
    if (sigaction(SIGABRT, NULL, &ac) < 0)
        err_sys("sigaction error");

    if (ac.sa_handler == SIG_IGN) {
        ac.sa_handler = SIG_DFL;

    } else if (ac.sa_handler == SIG_DFL) {
        fflush(NULL);
        goto raise;
    }

    /* Install signal handler. */
    if (sigaction(SIGABRT, &ac, NULL) < 0)
        err_sys("sigaction error");

raise:

    raise(SIGABRT);

    fflush(NULL);
    ac.sa_flags = 0;
    ac.sa_handler = SIG_DFL;

    if (sigaction(SIGABRT, &ac, NULL) < 0)
        err_sys("sigaction error");

    /*
     * We block all signals other than SIGABRT again
     * in case of the signal mask being changed in the signal handler.
     */
    if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0)
        err_sys("sigprocmask error");

    raise(SIGABRT);

    exit(1);
}

static void
sig_abort(int signo)
{
    printf("signal: %d caught\n", signo);
}

