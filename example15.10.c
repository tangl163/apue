#include "common.h"
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

#define MY_MUTEX_PRIVATE 0
#define MY_MUTEX_SHARED  1

#define SEM_TYPE_BIBARY 1

#define NUM_THREADS 3

typedef struct my_mutex_lock_t {
    sem_t *semp;
} my_mutex_lock_t;

static int my_mutex_init(my_mutex_lock_t *mutex, int shared);
static int my_mutex_destroy(my_mutex_lock_t *mutex);
static int my_mutex_lock(my_mutex_lock_t *mutex);
static int my_mutex_trylock(my_mutex_lock_t *mutex);
static int my_mutex_unlock(my_mutex_lock_t *mutex);
static int my_mutex_timedlock(my_mutex_lock_t *mutex, const struct timespec *tsptr);

static void *start_routine(void *arg);
static void thread_init(void);

static pthread_once_t flag = PTHREAD_ONCE_INIT;
static my_mutex_lock_t mutex;
static pthread_barrier_t barrier;

int
main(void)
{
    int i, err;
    pthread_t tid;

    err = pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);
    if (err != 0) {
        errno = err;
        err_sys("pthread_barrier_init error");
    }

    for (i = 0; i < NUM_THREADS; i++) {
        err = pthread_create(&tid, NULL, start_routine, (void *)(long)i);
        if (err != 0) {
            errno = err;
            err_sys("pthread_create error");
        }
    }

    pthread_barrier_wait(&barrier);
    pthread_barrier_destroy(&barrier);

    my_mutex_destroy(&mutex);

    exit(0);
}

static void *
start_routine(void *arg)
{
    pthread_once(&flag, thread_init);

    printf("In thread: %ld\n", (long)arg);
    my_mutex_lock(&mutex);
    printf("Locked\n");
    my_mutex_unlock(&mutex);
    printf("Unlock\n");

    pthread_barrier_wait(&barrier);
    return (void *)0;
}

static void
thread_init(void)
{
    my_mutex_init(&mutex, MY_MUTEX_PRIVATE);
}

static int
my_mutex_init(my_mutex_lock_t *mutex, int shared)
{
    mutex->semp = malloc(sizeof(sem_t));
    if (mutex->semp == NULL) {
        err_msg("malloc error");
        return -1;
    }

    if (sem_init(mutex->semp, shared, SEM_TYPE_BIBARY) < 0) {
        err_msg("sem_init error");
        return -1;
    }

    return 0;
}

static int
my_mutex_destroy(my_mutex_lock_t *mutex)
{
    if (sem_destroy(mutex->semp) != 0) {
        err_msg("sem_destroy error");
        return -1;
    }

    free(mutex->semp);

    return 0;
}

static int
my_mutex_lock(my_mutex_lock_t *mutex)
{
    if (sem_wait(mutex->semp) != 0) {
        err_msg("sem_wait error");
        return -1;
    }

    return 0;
}

static int
my_mutex_trylock(my_mutex_lock_t *mutex)
{
    if (sem_trywait(mutex->semp) != 0) {
        err_msg("sem_trywait error");
        return -1;
    }

    return 0;
}

static int
my_mutex_timedlock(my_mutex_lock_t *mutex, const struct timespec *tsptr)
{
    if (sem_timedwait(mutex->semp, tsptr) != 0) {
        err_msg("sem_timedwait error");
        return -1;
    }

    return 0;
}

static int
my_mutex_unlock(my_mutex_lock_t *mutex)
{
    if (sem_post(mutex->semp) != 0) {
        err_msg("sem_post error");
        return -1;
    }

    return 0;
}

