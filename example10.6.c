#include "common.h"
#include <pwd.h>
#include <signal.h>

static void sig_alarm(int);

int
main(void)
{
    struct passwd *ptr;

    if (signal(SIGALRM, sig_alarm) == SIG_ERR)
        err_sys("install signal for SIGALRM error");

    alarm(1);

    while (TRUE) {
        if ((ptr = getpwnam("paul")) == NULL)
            err_sys("getpwnam erro");

        if (strcmp(ptr->pw_name, "paul") != 0)
            printf("username: %s, passwd: %s\n", ptr->pw_name, ptr->pw_passwd);
    }
}

static void
sig_alarm(int signo)
{
    struct passwd *ptr;

    printf("sigal caught: %d\n", signo);

    if ((ptr = getpwnam("root")) == NULL)
        err_sys("getpwnam(root) error");

    alarm(1);
}

