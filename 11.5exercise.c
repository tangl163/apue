#include "common.h"
#include <pthread.h>

struct my_barrier_t {
    unsigned int     b_count;
    unsigned int     b_nwait;
    pthread_mutex_t  b_lock;
    pthread_cond_t   b_cond;
};

static void *start_routine(void *arg);
static int my_barrier_init(struct my_barrier_t *barrier, unsigned int count);
static int my_barrier_wait(struct my_barrier_t *barrier);

static struct my_barrier_t barrier;

/**
 * 11.5 What synchronization primitives would you need to implement a barrier?
 * Provide an implementation of the pthread_barrier_wait function.
 */
int
main(void)
{
    long i;
    int err, ret;
    pthread_t tid;

    my_barrier_init(&barrier, 4);

    for (i = 0; i < 3; i++) {
        err = pthread_create(&tid, NULL, start_routine, (void *)i);
        if (err != 0) {
            printf("pthread_create error: %s\n", strerror(err));
            exit(0);
        }
    }

    ret = my_barrier_wait(&barrier);

    sleep(2);
    printf("ret: %d\n", ret);

    exit(0);
}

static void *
start_routine(void *arg)
{
    int ret;

    ret = my_barrier_wait(&barrier);

    printf("I'm thread: %lu with arg: %ld ret: %d\n",
            (unsigned long)pthread_self(), (long)(arg), ret);

    return (void *)0;
}

static int
my_barrier_init(struct my_barrier_t *barrier, unsigned int count)
{
    int err;

    if ((err = pthread_mutex_init(&barrier->b_lock, NULL)) != 0)
        return err;

    if ((err = pthread_cond_init(&barrier->b_cond, NULL)) != 0) {
        pthread_mutex_unlock(&barrier->b_lock);
        return err;
    }

    barrier->b_count = count;
    barrier->b_nwait = 0;

    return 0;
}

static int
my_barrier_wait(struct my_barrier_t *barrier)
{
    pthread_mutex_lock(&barrier->b_lock);
    barrier->b_nwait++;

    if (barrier->b_nwait == barrier->b_count) {
        pthread_mutex_unlock(&barrier->b_lock);
        pthread_cond_broadcast(&barrier->b_cond);
        return PTHREAD_BARRIER_SERIAL_THREAD;
    }

    pthread_cond_wait(&barrier->b_cond, &barrier->b_lock);
    pthread_mutex_unlock(&barrier->b_lock);

    return 0;
}

