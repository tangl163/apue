#include "common.h"
#include <signal.h>
#include <pthread.h>

static void *start_rt(void *);
static void sig_handler(int);

static sigset_t mask;
static sigset_t emptyset;

int
main(void)
{
    int err;
    sigset_t set;
    pthread_t tid;

    sigemptyset(&mask);
    sigemptyset(&emptyset);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    signal(SIGINT, sig_handler);

    err = pthread_create(&tid, NULL, start_rt, NULL);
    if (err != 0)
        err_exit(err, "pthread_create error");

    sigsuspend(&emptyset);

    exit(0);
}

static void *
start_rt(void *arg)
{
    int signo;

    sigwait(&mask, &signo);

    printf("signo: %d was caught by thread\n", signo);

    return NULL;
}

static void
sig_handler(int signo)
{
    printf("signo: %d was caught by signal handler\n", signo);
}

