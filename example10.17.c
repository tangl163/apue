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
    struct sigaction ac, oldac;
    sigset_t unblockset;

    ac.sa_flags = 0;
    sigemptyset(&ac.sa_mask);
    sigemptyset(&unblockset);
    sigaddset(&unblockset, SIGABRT);

    if (sigprocmask(SIG_UNBLOCK, &unblockset, NULL) < 0)
        err_sys("sigprocmask error");
    
    if (sigaction(SIGABRT, NULL, &oldac) < 0)
        err_sys("sigaction error");

    /* If SA_SIGINFO is set, SIGABRT handler would be sa_sigaction. */
    if (oldac.sa_flags & SA_SIGINFO) {
        ac.sa_flags |= SA_SIGINFO;
        ac.sa_sigaction = oldac.sa_sigaction;

    } else if (oldac.sa_handler == SIG_IGN) {
        ac.sa_handler = SIG_DFL;

    } else {
        ac.sa_handler = oldac.sa_handler;
    }

    /* Install signal handler. */
    if (sigaction(SIGABRT, &ac, NULL) < 0)
        err_sys("sigaction error");

    raise(SIGABRT);

    _exit(127);
}

static void
sig_abort(int signo)
{
    printf("signal: %d caught\n", signo);
}

