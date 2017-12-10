#include "common.h"
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

int
main(void)
{
    int err;
    char buf[BUFSIZ];
    struct tm *tmp;
    struct timespec tsp;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&lock);

    if (clock_gettime(CLOCK_REALTIME, &tsp) < 0)
        err_sys("clock_gettime error");

    if ((tmp = localtime(&tsp.tv_sec)) == NULL)
        err_sys("localtime error");

    strftime(buf, BUFSIZ, "%r", tmp);

    printf("before timedlock: %s\n", buf);

    tsp.tv_sec += 10;
    if ((err = pthread_mutex_timedlock(&lock, &tsp)) != 0)
        printf("pthread_mutex_timedlock failure: %s\n", strerror(err));

    if (clock_gettime(CLOCK_REALTIME, &tsp) < 0)
        err_sys("clock_gettime error");

    if ((tmp = localtime(&tsp.tv_sec)) == NULL)
        err_sys("localtime error");

    strftime(buf, BUFSIZ, "%r", tmp);

    printf("after timedlock: %s\n", buf);

    pthread_mutex_unlock(&lock);
    if ((err = pthread_mutex_destroy(&lock)) != 0)
        printf("pthread_mutex_destroy failure %s\n", strerror(err));

    exit(0);
}

