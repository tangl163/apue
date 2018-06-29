#include "common.h"
#include <sys/ipc.h>
#include <sys/sem.h>

#define PROJECT_ID 7
#define RWRWRW S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH

int
main(int argc, char *argv[])
{
    key_t key;
    int semid, val;
    struct sembuf buf;

    if (argc != 2)
        err_quit("Usage: %s pathname", argv[0]);

    key = ftok(argv[1], PROJECT_ID);
    if (key < 0)
        err_sys("ftok error");

    semid = semget(key, 1, RWRWRW | IPC_CREAT | IPC_EXCL);
    if (semid < 0)
        err_sys("semget error");

    printf("semid: %d\n", semid);

    val = semctl(semid, 0, GETVAL);
    if (val < 0)
        err_sys("semctl error");

    printf("before semop\nsemval: %hd\n", val);

    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;

    if (semop(semid, &buf, 1) < 0)
        err_sys("semop error");

    val = semctl(semid, 0, GETVAL);
    if (val < 0)
        err_sys("semctl error");

    printf("after semop\nsemval: %hd\n", val);

    if (semctl(semid, 0, IPC_RMID) < 0)
        err_sys("semctl error");

    exit(0);
}

