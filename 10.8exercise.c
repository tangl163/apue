#include "common.h"
#include <signal.h>

static void sig_int(int signo, siginfo_t *info, void *context);

/**
 * 10.8 Why do you think the siginfo structure(Section 10.14)
 * includes the real user ID, instead of the effective user ID, in the si_uid ﬁeld?
 */

int
main(void)
{
    struct sigaction sa;

    sa.sa_flags = 0;
    sa.sa_flags |= SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sig_int;

    if (sigaction(SIGINT, &sa, NULL) < 0)
        err_sys("sigaction error");

    raise(SIGINT);

    exit(0);
}

static void
sig_int(int signo, siginfo_t *info, void *context)
{
    printf("getuid: %d, geteuid: %d\n", getuid(), geteuid());
    printf("uid: %d\n", info->si_uid);
}

