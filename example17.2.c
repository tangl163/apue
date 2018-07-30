#include "common.h"
#include <pthread.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <poll.h>

#define NTHREADS 4
#define MAXSIZE 512

struct my_msg_t {
    long type;
    char buf[MAXSIZE];
};

struct thread_arg_t {
    int msgid;
    int sockfd;
    pthread_t tid;
    long number;
};

static void *start_routine(void *arg);

int
main(void)
{
    int i, n, count, ret;
    int msgid;
    int sockfd[2];
    char buf[MAXSIZE];
    pthread_attr_t attr;
    struct my_msg_t msg;
    struct pollfd pollfd;
    struct thread_arg_t args[NTHREADS];

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    msgid = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR);
    if (msgid == -1)
        err_sys("msgget error");

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1) {
        msgctl(msgid, IPC_RMID, NULL);
        err_sys("socketpari error");
    }

    /* 0 for read */
    pollfd.fd = sockfd[0];
    pollfd.events = POLLIN;

    for (i = 0; i < NTHREADS; i++) {
        args[i].msgid = msgid;
        args[i].sockfd = sockfd[1];
        args[i].number = i + 1;

        if (pthread_create(&args[i].tid, &attr, start_routine, (void *)&args[i])) {
            err_msg("pthread_create error");
            goto cleanup;
        }
    }

    pthread_attr_destroy(&attr);

    for (count = 0 ; /* void */ ; count++) {
        n = read(STDIN_FILENO, msg.buf, MAXSIZE);
        if (n == -1) {
            err_msg("read error");
            goto cleanup;
        }

        if (n == 0) {
            fprintf(stderr, "EOF read\n");
            break;
        }

        msg.type = args[count % NTHREADS].number;

        if (msgsnd(msgid, (void *)&msg, n, 0) == -1) {
            err_msg("msgsnd error");
            goto cleanup;
        }

        if (poll(&pollfd, 1, -1) == -1) {
            err_msg("poll error");
            goto cleanup;
        }

        if (pollfd.revents & POLLERR) {
            err_msg("fd POLLERR");
            goto cleanup;
        }

        if (pollfd.revents & POLLHUP) {
            err_msg("POLLHUP");
            goto cleanup;
        }

        if (pollfd.revents & POLLIN) {
            ret = recv(sockfd[0], buf, MAXSIZE, 0);
            if (ret == -1) {
                err_msg("recv error");
                goto cleanup;
            }

            if (ret == 0) {
                fprintf(stderr, "peer shutdown\n");
                break;
            }

            if (write(STDOUT_FILENO, buf, ret) != ret) {
                err_msg("write error");
                goto cleanup;
            }
        }
    }

cleanup:

    msgctl(msgid, IPC_RMID, NULL);
    close(sockfd[0]);
    close(sockfd[1]);

    exit(0);
}

static void *
start_routine(void *arg)
{
    int size;
    struct my_msg_t msg;
    struct thread_arg_t *ptr;

    ptr = (struct thread_arg_t *)arg;

    for ( ;; ) {
        size = msgrcv(ptr->msgid, (void *)&msg, MAXSIZE, ptr->number, 0);
        if (size == -1) {
            fprintf(stderr, "msgrcv error. thread_id: %ld\n", (long)ptr->tid);
            break;
        }

        printf("%d bytes received\n", size);

        if (send(ptr->sockfd, msg.buf, size, 0) != size) {
            fprintf(stderr, "send error. thread_id: %ld\n", (long)ptr->tid);
            break;
        }
    }

    return (void *)0;
}

