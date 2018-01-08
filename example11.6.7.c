#include "common.h"
#include <pthread.h>

int
main(void)
{
    int err;
    pthread_spinlock_t spin;

    pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);
    pthread_spin_lock(&spin);

    if ((err = pthread_spin_lock(&spin)) != 0) {
        printf("pthread_spin_lock error: %s\n", strerror(err));
        exit(0);
    }

    exit(0);
}

