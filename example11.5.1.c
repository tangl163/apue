#include "common.h"
#include <pthread.h>

static void *thread_start(void *arg);

int
main(void)
{
    int err;
    void *status;
    pthread_t tid;

    if (pthread_create(&tid, NULL, thread_start, NULL) != 0)
        err_sys("pthread_create error");

    if (pthread_detach(tid) != 0)
        err_sys("pthread_detach error");

    if ((err = pthread_join(tid, &status)) != 0)
        printf("pthread_join error: %s\n", strerror(err));

    exit(0);
}

static void *
thread_start(void *arg)
{
    printf("In thread %p\n", arg);

    sleep(1);

    return (void *)0;
}

