#include "common.h"
#include <sys/wait.h>

#define FIFO_PATH "/tmp/tmp_fifo"

int
main(void)
{
    int n, fd;
    pid_t pid;
    long capacity;

    if (mkfifo(FIFO_PATH, S_IRUSR | S_IWUSR) != 0)
        err_sys("mkfifo error");

    TELL_WAIT();

    pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
        if (fd < 0)
            err_sys("Child: open error");

        TELL_PARENT();
        WAIT_PARENT();
        exit(0);
    } else {

        WAIT_CHILD();
        fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
        if (fd < 0)
            err_sys("Parent: open error");

        for (capacity = 0; /* void */ ; capacity++) {
            n = write(fd, "a", 1);
            if (n != 1) {
                printf("write return %d\n", n);
                break;
            }
        }

        TELL_CHILD(pid);
    }

    while (waitpid(pid, NULL, 0) < 0) {
        if (errno != EINTR)
            err_sys("waitpid error");
    }

    unlink(FIFO_PATH);

    printf("Capacity: %ld\n", capacity);
    printf("PIPE_BUF: %d\n", PIPE_BUF);

    exit(0);
}

