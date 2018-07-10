#include "common.h"
#include <sys/ipc.h>
#include <sys/shm.h>

static long update(long *arg);

/**
 * 15.15 Redo the program in Figure 15.33 using the XSI shared memory functions
 * from Section 15.9 instead of the shared memory-mapped region.
 */
int
main(int argc, char *argv[])
{
    long n;
    int i, shmid;
    key_t key;
    pid_t pid;
    void *addr;

    if (argc != 2)
        err_quit("Usage: %s pathname", argv[0]);

    key = ftok(argv[1], rand());
    if (key == -1)
        err_sys("ftok error");

    shmid = shmget(key, sizeof(long), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (shmid == -1)
        err_sys("shmget error");

    addr = shmat(shmid, 0, 0);
    if (addr == (void *)-1)
        err_sys("shmat error");

    TELL_WAIT();

    pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        for (i = 0; i < 1000; i += 2) {
            n = update((long *)addr);
            if (n != i)
                err_quit("expect: %d actual: %ld\n", i, n);

            TELL_PARENT();
            WAIT_PARENT();
        }

        exit(0);

    } else {
        for (i = 1; i < 1000; i += 2) {
            WAIT_CHILD();

            n = update((long *)addr);
            if (n != i)
                err_quit("expect: %d actual: %ld\n", i, n);

            TELL_CHILD(pid);
        }
    }

    if (shmdt(addr) == -1)
        err_sys("shmdt error");

    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        err_sys("shmctl error");

    exit(0);
}

static long
update(long *arg)
{
    return (*arg)++;
}

