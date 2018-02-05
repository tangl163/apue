#include "common.h"
#include <signal.h>
#include <pthread.h>

static void thread_init(void);
static int putenv_r(char *);
static void copyoldenv(void);

extern char **environ;

static pthread_once_t initflag = PTHREAD_ONCE_INIT;
static pthread_mutex_t env_lock;
static sigset_t mask;

/**
 * 12.2 Implement putenv_r, areentrant version of putenv. Make sure
 * that your implementation is async-signal safe as well as thread-safe.
 */
int
main(int argc, char *argv[])
{
    int i;

    if (argc < 2)
        err_quit("Usage: %s <envstring>", argv[0]);

    for (i = 1; argv[i]; i++)
        putenv_r(argv[i]);

    for (i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);

    exit(0);
}

static void
thread_init(void)
{
    copyoldenv();
    sigfillset(&mask);
    pthread_mutex_init(&env_lock, NULL);
}

static void
copyoldenv(void)
{
    int i, len;
    char **p, *str;

    for (i = 0; environ[i]; i++)
        ;

    p = malloc(sizeof *environ * (i + 1));
    if (p == NULL)
        err_sys("malloc error");

    for (i = 0; environ[i]; i++) {

        len = strlen(environ[i]);
        str = malloc(sizeof *str * (len + 1));
        if (str == NULL)
            err_sys("malloc error");

        strncpy(str, environ[i], len);
        str[len] = 0;
        p[i] = str;
    }

    p[i] = NULL;
    environ = p;
}

static int
putenv_r(char *str)
{
    sigset_t oldmask;
    int len, nlen, i;
    char *ptr, *edge;

    ptr = edge = NULL;
    pthread_once(&initflag, thread_init);

    /* we block all signals to make sure it won't be interrupted */
    if (pthread_sigmask(SIG_BLOCK, &mask, &oldmask) != 0) {
        err_msg("pthread_sigmask error");
        return -1;
    }

    /* using a mutex to make it thread-safe */
    pthread_mutex_lock(&env_lock);

    len = strlen(str);
    if (len == 0)
        goto success;

    /* replace env */
    if ((edge = strchr(str, '=')) != NULL) {

        nlen = edge - str;

        for (i = 0; environ[i]; i++) {
            if (strncmp(str, environ[i], nlen) == 0
                && environ[i][nlen] == '=') {
                ptr = environ[i];
                break;
            }
        }

        /**
         * If it exists in the environ, we just replace it with a new string.
         * Otherwise, we need to append it at the end of environ.
         */
        if (ptr) {
            environ[i] = str;
            free(ptr);
            goto success;
        } else {
            environ = realloc(environ, sizeof *environ * (i + 1));
            if (environ == NULL) {
                err_msg("realloc failed");
                goto fail;
            }

            environ[i] = str;
            environ[i+1] = NULL;
            goto success;
        }

    } else {
        for (i = 0; environ[i]; i++) {
            if (strncmp(str, environ[i], len) == 0
                && environ[i][len] == '=') {
                ptr = environ[i];
                break;
            }
        }

        /* In this case, we delete an env */
        if (ptr) {
            do {
                environ[i] = environ[i+1];
                i++;
            } while (environ[i]);
        }
    }

success:
    pthread_mutex_unlock(&env_lock);
    pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
    return 0;

fail:
    pthread_mutex_unlock(&env_lock);
    pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
    return -1;
}

