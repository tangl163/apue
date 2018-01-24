#include "common.h"
#include <pthread.h>

static void *start_rt(void *arg);

static pthread_mutex_t lock;

int
main(void)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;
    pthread_mutexattr_t mtx_attr;

    pthread_mutexattr_init(&mtx_attr);
    pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&lock, &mtx_attr);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if ((err = pthread_create(&tid, &attr, start_rt, NULL)) != 0)
        err_exit(err, "pthread_create error");

    if ((err = pthread_mutex_unlock(&lock)) != 0)
        err_exit(err, "pthread_mutex_unlock error");

    sleep(3);
    printf("%d\n", err);
    exit(0);
}

static void *
start_rt(void *arg)
{
    pthread_mutex_lock(&lock);

    sleep(2);

    pthread_mutex_unlock(&lock);

    return (void *)0;
}

