#include "common.h"
#include <signal.h>

int main(void)
{
    sigset_t set1, set2;

    sigfillset(&set1);

    if (sigprocmask(SIG_BLOCK, &set1, NULL) < 0)
        err_sys("sigpromask error");

    sigemptyset(&set2);

    if (sigaddset(&set2, SIGINT) < 0)
        err_sys("sigaddset error");

    if (sigprocmask(SIG_UNBLOCK, &set2, NULL) < 0)
        err_sys("sigpromask error");

    pr_sigmask("blocked mask");

    exit(0);
}

