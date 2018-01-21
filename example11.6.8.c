#include "common.h"
#include <pthread.h>

#define N_NUM 8000000L

static void *start_thread(void *arg);
static void swap(long *v, size_t i, size_t j);
static void quicksort(long *v, size_t left, size_t right);
static void merge(void);

static size_t nthread;    /* number of threads */
static size_t nperth;     /* number to sort per thread */
static pthread_barrier_t barrier;

static long num[N_NUM];
static long sorted_num[N_NUM];

int
main(int argc, char *argv[])
{
    int err;
    size_t i;
    pthread_t tid;

    if (argc != 2)
        err_quit("Usage: %s <nthread>", argv[0]);

    nthread = (size_t)atol(argv[1]);
    nperth = N_NUM / nthread;

    for (i = 0; i < N_NUM; i++)
        num[i] = random();

    err = pthread_barrier_init(&barrier, NULL, nthread + 1);
    if (err != 0) {
        printf("pthread_barrier_init error: %s\n", strerror(err));
        exit(0);
    }

    for (i = 0; i < nthread; i++) {
        err = pthread_create(&tid, NULL, start_thread, (void *)(i * nperth));
        if (err != 0) {
            printf("pthread_create error: %s\n", strerror(err));
            exit(0);
        }
    }

    pthread_barrier_wait(&barrier);

    merge();
    
    exit(0);
}

static void *
start_thread(void *arg)
{
    size_t start, end;

    start = (size_t)arg;
    end = start + nperth - 1;

    quicksort(num, start, end);
    pthread_barrier_wait(&barrier);

    pthread_exit((void *)0);
}

static void
quicksort(long *v, size_t left, size_t right)
{
    size_t i, last;

    if (left >= right)
        return;

    swap(v, left, (left + right) / 2);
    last = left;

    for (i = left+1; i <= right; i++) {
        if (v[i] < v[left])
            swap(v, i, ++last);
    }

    swap(v, left, last);
    quicksort(v, left, last - 1);
    quicksort(v, last + 1, right);
}

static void
swap(long *v, size_t i, size_t j)
{
    long tmp;

    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

static void
merge(void)
{
    size_t i, j;
    size_t index, minbucket;
    long bucket[nthread], minval;

    for (i = 0; i < nthread; i++)
        bucket[i] = nperth * i;

    for (i = 0; i < N_NUM; i++) {

        minval = LONG_MAX;
        for (j = 0; j < nthread; j++) {

            index = bucket[j];
            if (index >= ((j+1) * nperth))
                continue;

            if (num[index] < minval) {
                minbucket = j;
                minval = num[index];
            }
        }

        sorted_num[i] = minval;
        bucket[minbucket]++;
    }
}

