#include "common.h"
#include <pthread.h>

static void prepare(void);
static void parent(void);
static void child(void);
static void * start_rt(void *arg);

static pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

/**
 * 12.1 Run the program in Figure12.17 on a Linux system, but redirect
 * the output into a ﬁle. Explain the results.
 */
int
main(void)
{
    int err;
    pid_t pid;
    pthread_t tid;

    err = pthread_atfork(prepare, parent, child);
    if (err != 0)
        err_exit(err, "pthread_atfork error");

    err = pthread_create(&tid, NULL, start_rt, NULL);
    if (err != 0)
        err_exit(err, "pthread_create error");

    sleep(2);
    printf("parent is about to fork\n");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        printf("child returned from fork\n");
    } else {
        printf("parent returned from fork\n");
    }

    exit(0);
}

static void *
start_rt(void *arg)
{
    printf("In thread\n");
    pause();
    return arg;
}

static void
prepare(void)
{
    printf("Prepare locking\n");
    pthread_mutex_lock(&lock1);
    pthread_mutex_lock(&lock2);
}

static void
parent(void)
{
    printf("Parent unlocking\n");
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
}

static void
child(void)
{
    printf("Child unlocking\n");
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
}

