#include "common.h"
#include <pthread.h>
#include <signal.h>
#include <aio.h>

static void sigev_notify(union sigval value);
static int sigevent_init(struct sigevent *sigev);
static void rot13_translate(char *str, long len);

int
main(int argc, char *argv[])
{
    int fd;
    long ret;
    char buf[BUFSIZ];
    struct aiocb cb;
    const struct aiocb *list[] = {&cb};

    if (argc != 2)
        err_quit("Usage: %s filename", argv[0]);

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
        err_sys("open for %s error", argv[1]);

    if (sigevent_init(&cb.aio_sigevent) < 0) {
        printf("sigevern_init error\n");
        exit(0);
    }

    cb.aio_fildes = fd;
    cb.aio_offset = 0;
    cb.aio_buf = buf;
    cb.aio_nbytes = BUFSIZ;
    cb.aio_reqprio = 0;

    printf("begin to read\n");

    ret = aio_read(&cb);
    if (ret < 0)
        err_sys("aio_read error");

    ret = aio_suspend(list, 1, NULL);
    if (ret < 0)
        err_sys("aio_suspend error");

    printf("read finish\n");

    ret = aio_return(&cb);
    if (ret < 0)
        err_sys("aio_return error");
    else
        printf("%ld bytes read from file\n", ret);

    rot13_translate(buf, ret);

    cb.aio_nbytes = ret;

    printf("begin to write\n");

    ret = aio_write(&cb);
    if (ret < 0)
        err_sys("aio_write error");

    ret = aio_suspend(list, 1, NULL);
    if (ret < 0)
        err_sys("aio_suspend error");

    printf("write finish\n");
    ret = aio_return(&cb);
    if (ret < 0)
        err_sys("aio_return error");

    printf("write finished. %ld bytes written\n", ret);

    sleep(1);
    exit(0);
}

static void
rot13_translate(char *str, long len)
{
    while (len--) {
        if (isalpha(*str)) {
            if (*str >= 'n')
                *str -= 13;
            else if (*str >= 'a')
                *str += 13;
            else if (*str >= 'N')
                *str -= 13;
            else
                *str += 13;
        }
        str++;
    }
}

static int
sigevent_init(struct sigevent *sigev)
{
    int err;
    pthread_attr_t *attr;

    attr = malloc(sizeof(pthread_attr_t));
    if (attr == NULL) {
        printf("malloc error: %s\n", strerror(errno));
        return -1;
    }

    err = pthread_attr_init(attr);
    if (err) {
        printf("pthread_attr_init error: %s\n", strerror(err));
        free(attr);
        return -1;
    }

    err = pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED);
    if (err) {
        printf("pthread_attr_setdetachstate error: %s\n", strerror(err));
        return -1;
    }

    sigev->sigev_notify = SIGEV_THREAD;
    sigev->sigev_signo = 0;
    sigev->sigev_value.sival_int = 0;
    sigev->sigev_notify_function = sigev_notify;
    sigev->sigev_notify_attributes = attr;

    return 0;
}

static void
sigev_notify(union sigval value)
{
    fprintf(stderr, "aio has complete. sigval.sival_int = %d\n", value.sival_int);
}

