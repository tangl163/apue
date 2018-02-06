#include "common.h"
#include <pthread.h>
#include <signal.h>

static void thread_init(void);
static char *mygetenv(char *str);

extern char **environ;

static sigset_t mask;
static pthread_key_t key;
static pthread_once_t initflag = PTHREAD_ONCE_INIT;
static pthread_mutex_t env_lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * 12.3 Can you make the getenv function shown in Figure 12.13 async-signal safe
 * by blocking signals at the beginning of the function and restoring the previous
 * signal mask before returning? Explain.
 */
int
main(int argc, char *argv[])
{
    char *p;

    if (argc != 2)
        err_quit("Usage: %s <envstring>", argv[0]);

    p = mygetenv(argv[1]);
    if (p != NULL)
        printf("value: %s\n", p);
    else if (errno != 0)
        printf("mygetenv error: %s\n", strerror(errno));
    else
        printf("No such env: %s\n", argv[1]);

    exit(0);
}

static void
thread_init(void)
{
    sigfillset(&mask);
    pthread_key_create(&key, free);
}

static char *
mygetenv(char *str)
{
    int i;
    size_t len;
    char *buf;
    sigset_t oldmask;

    pthread_once(&initflag, thread_init);
    pthread_sigmask(SIG_BLOCK, &mask, &oldmask);
    pthread_mutex_lock(&env_lock);

    buf = (char *)pthread_getspecific(key);
    if (buf == NULL) {

        buf = malloc(sizeof *buf * MAXLINE);
        if (buf == NULL) {
            errno = ENOMEM;
            goto result;
        }
        pthread_setspecific(key, buf);
    }

    len = strlen(str);

    for (i = 0; environ[i]; i++) {
        if (strncmp(str, environ[i], len) == 0
            && environ[i][len] == '=') {
            strncpy(buf, &environ[i][len+1], MAXLINE-1);
            break;
        }
    }

    if (environ[i] == NULL)
        buf = NULL;

result:
    pthread_mutex_unlock(&env_lock);
    pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
    return buf;
}

