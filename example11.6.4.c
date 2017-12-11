#include "common.h"
#include <pthread.h>

int
main(void)
{
    int err;
    pthread_rwlock_t lock;
    unsigned long count = 0;

    pthread_rwlock_init(&lock, NULL);

    while (TRUE) {
        if ((err = pthread_rwlock_rdlock(&lock)) != 0)
            break;
        count++;
    }

    printf("maximal number of readlock readched: %lu\n", count);
    printf("%s\n", strerror(err));

    exit(0);
}

