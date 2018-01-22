#include "common.h"
#include <pthread.h>

static void *thread_start(void *arg);
static pthread_cond_t ready = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int
main(void)
{
    int err;
    pthread_t tid;

    if ((err = pthread_create(&tid, NULL, thread_start, NULL)) != 0) {
        printf("pthread_create error: %s\n", strerror(err));
        exit(0);
    }

    sleep(1);

    pthread_mutex_lock(&lock);
    pthread_cond_signal(&ready);
    pthread_mutex_unlock(&lock);

    if ((err = pthread_join(tid, NULL)) != 0) {
        printf("pthread_join error: %s\n", strerror(err));
        exit(0);
    }

    printf("All goes well\n");
    exit(0);
}

static void *
thread_start(void *arg)
{
    int err;

    pthread_mutex_lock(&lock);

    if ((err = pthread_cond_wait(&ready, &lock)) != 0) {
        printf("pthread_cond_wait error: %s\n", strerror(err));
        pthread_exit((void *)0);
    }

    printf("lock reaquired\n");
    pthread_mutex_unlock(&lock);

    return arg;
}

