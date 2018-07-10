#include "common.h"
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_LEN 128

struct msg_t {
    long mtype;
    int number;
};

/**
 * 15.12 Write a program that does the following. Execute a loop five
 * times: create a message queue, print the queue identifier,
 * delete the message queue. Then execute the next loop five
 * times: create a message queue with a key of IPC_PRIVATE, and
 * place a message on the queue. After the program terminates, look
 * at the message queues using ipcs(1). Explain what is happening
 * with the queue identifiers.
 */
int
main(int argc, char *argv[])
{
    key_t key;
    int i, msgid;
    struct msg_t mymsg;

    if (argc != 2)
        err_quit("Usage: %s path", argv[0]);

    key = ftok(argv[1], rand());
    if (key < 0)
        err_sys("ftok error");

    for (i = 0; i < 5; i++) {
        msgid = msgget(key, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        if (msgid == -1)
            err_sys("msgget error");

        printf("identifier: %d\n", msgid);

        if (msgctl(msgid, IPC_RMID, NULL) == -1)
            err_sys("msgctl error");
    }

    mymsg.mtype = getpid();

    for (i = 0; i < 5; i++) {
        msgid = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR);
        if (msgid == -1)
            err_sys("msgget error");

        mymsg.number = i;

        if (msgsnd(msgid, (void *)&mymsg, sizeof(struct msg_t), 0) == -1)
            err_sys("msgsnd error");
    }

    exit(0);
}

