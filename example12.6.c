#include "common.h"
#include <pthread.h>

struct msg_t {
    char s[MAXLINE];
};

static void *start_rt(void *arg);
static void thread_init(void);
static void destructor(void *);

static pthread_once_t initflag = PTHREAD_ONCE_INIT;
static pthread_key_t key;

int
main(void)
{
    int err;
    pthread_t tid;
    struct msg_t *p;

    err = pthread_create(&tid, NULL, start_rt, NULL);
    if (err != 0)
        err_exit(err, "pthread_create error");

    pthread_once(&initflag, thread_init);

    p = (struct msg_t *)pthread_getspecific(key);
    if (p == NULL) {
        if ((p = malloc(sizeof *p)) == NULL)
            err_sys("malloc error");
        pthread_setspecific(key, p);
    }

    strncpy(p->s, "I'm main thread", MAXLINE-1);

    sleep(2);
    pthread_exit(NULL);
}

static void *
start_rt(void *arg)
{
    struct msg_t *p;

    pthread_once(&initflag, thread_init);

    p = (struct msg_t *)pthread_getspecific(key);
    if (p == NULL) {

        if ((p = malloc(sizeof *p)) == NULL) {
            err_msg("malloc error");
            return NULL;
        }

        pthread_setspecific(key, p);
    }

    strncpy(p->s, "I'm a thread", MAXLINE-1);

    return NULL;
}

static void
thread_init(void)
{
    pthread_key_create(&key, destructor);
}

static void
destructor(void *msg)
{
    printf("%s\n", ((struct msg_t *)msg)->s);
    free(msg);
}

