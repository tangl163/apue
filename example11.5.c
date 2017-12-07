#include "common.h"
#include <pthread.h>

static void cleanup_handler1(void *str);
static void cleanup_handler2(void *str);
static void cleanup_handler3(void *str);

static void *thread_start(void *arg);

int
main(void)
{
    void *status;
    pthread_t tid;

    if (pthread_create(&tid, NULL, thread_start, NULL) != 0)
        err_sys("pthread_create error");

    if (pthread_cancel(tid) != 0)
        err_sys("pthread_cancel error");

    if (pthread_join(tid, &status) != 0)
        err_sys("pthread_join error");

    if (status == PTHREAD_CANCELED)
        printf("thread being canceled\n");
    else
        printf("thread exit code: %ld\n", (long)status);

    exit(0);
}

static void *
thread_start(void *arg)
{
    pthread_cleanup_push(cleanup_handler1, "hi");
    pthread_cleanup_push(cleanup_handler2, "hello");
    pthread_cleanup_push(cleanup_handler3, "he");

    sleep(2);

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);

    return arg;
}

static void
cleanup_handler1(void *str)
{
    printf("I'm handler1. %s\n", (char *)str);
}

static void
cleanup_handler2(void *str)
{
    printf("I'm handler2. %s\n", (char *)str);
}

static void
cleanup_handler3(void *str)
{
    printf("I'm handler3. %s\n", (char *)str);
}

