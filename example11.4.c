#include "common.h"
#include <pthread.h>

static void *thread_start(void *arg);
static void pr_thread(char *msg);

int
main(void)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, thread_start, NULL) != 0)
        err_sys("pthread_create error");

    if (pthread_join(tid, NULL) != 0)
        err_sys("pthread_join error");

    pr_thread("In main thread");

    exit(0);
}

static void *
thread_start(void *arg)
{
    pr_thread("In new thread");

    return arg;
}

static void
pr_thread(char *msg)
{
    pthread_t tid;

    tid = pthread_self();

    printf("%1$s: tid: %2$lu(0X%2$lX)\n", msg, (unsigned long)tid);
}

