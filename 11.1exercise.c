#include "common.h"
#include <pthread.h>

struct msg {
    pthread_t tid;
    char s[10];
};

static void *start_thread(void *arg);

/**
 * 11.1 Modify the example code shown in Figure11.4 to pass the
 * structure between the threads properly.
 */
int
main(void)
{
    int err;
    struct msg *p;
    pthread_t tid;

    if ((err = pthread_create(&tid, NULL, start_thread, NULL)) != 0) {
        printf("pthread_create error: %s\n", strerror(err));
        exit(0);
    }

    if ((err = pthread_join(tid, (void *)&p)) != 0) {
        printf("pthread_join error: %s\n", strerror(err));
        exit(0);
    }

    printf("A message from thread: %lu\n%s\n", (unsigned long)p->tid, p->s);

    exit(0);
}

static void *
start_thread(void *arg)
{
    struct msg *p;

    if ((p = malloc(sizeof *p)) == NULL)
        err_sys("malloc error");

    p->tid = pthread_self();
    strcpy(p->s, "hello");
    
    pthread_exit(p);
}

