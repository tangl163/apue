#include "common.h"
#include <pthread.h>

struct my_barrier_t {
    unsigned int     b_count;
    unsigned int     b_nwait;
    pthread_mutex_t  b_lock;
    pthread_cond_t   b_cond;
};

static int my_barrier_init(struct my_barrier_t *barrier, unsigned int count);
static int my_barrier_wait(struct my_barrier_t *barrier);

int
main(void)
{
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
    pthread_cond_wait(&barrier->b_cond, &barrier->b_lock);

}

