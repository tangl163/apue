#include "common.h"

int
main(void)
{
    long dtor_iterations, keys_max, stack_min, threads_max;

    if ((dtor_iterations = sysconf(_SC_THREAD_DESTRUCTOR_ITERATIONS)) < 0) {
        if (errno != 0)
            err_msg("sysconf for _SC_THREAD_DESTRUCTOR_ITERATIONS error");
    }

    if ((keys_max = sysconf(_SC_THREAD_KEYS_MAX)) < 0) {
        if (errno != 0)
            err_msg("sysconf for _SC_THREAD_KEYS_MAX error");
    }

    if ((stack_min = sysconf(_SC_THREAD_STACK_MIN)) < 0) {
        if (errno != 0)
            err_msg("sysconf for _SC_THREAD_STACK_MIN error");
    }

    if ((threads_max = sysconf(_SC_THREAD_THREADS_MAX)) < 0) {
        if (errno != 0)
            err_msg("sysconf for _SC_THREAD_THREADS_MAX error");
    }

    printf("PTHREAD_DESTRUCTOR_ITERATIONS: %ld\n", dtor_iterations);
    printf("PTHREAD_KEYS_MAX:              %ld\n", keys_max);
    printf("PTHREAD_STACK_MIN:             %ld\n", stack_min);
    printf("PTHREAD_THREADS_MAX:           %ld\n", threads_max);

    exit(0);
}

