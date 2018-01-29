#include "common.h"
#include <pthread.h>

static void * start_rt1(void *arg);
static void * start_rt2(void *arg);

int
main(void)
{
    int err;
    pthread_t tid1, tid2;

    err = pthread_create(&tid1, NULL, start_rt1, NULL);
    if (err != 0)
        err_exit(err, "pthread_create error");

    err = pthread_create(&tid2, NULL, start_rt2, NULL);
    if (err != 0)
        err_exit(err, "pthread_create error");

    pthread_cancel(tid1);
    pthread_cancel(tid2);

    sleep(3);

    exit(0);
}

static void *
start_rt1(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    sleep(2);

    printf("I'm thread1\n");

    return NULL;
}

static void *
start_rt2(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    sleep(2);

    printf("I'm thread2\n");

    return NULL;
}

