#include "common.h"
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    pid_t pid;
    char buf[PIPE_BUF];
    int fd, pipefd[2], n;

    if (argc != 2)
        err_quit("Usage: %s filename", argv[0]);

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
        err_sys("open for %s error", argv[1]);

    if (pipe(pipefd) < 0)
        err_sys("pipe error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        close(pipefd[1]);

        if (pipefd[0] != STDIN_FILENO) {

            if (dup2(pipefd[0], STDIN_FILENO) < 0)
                err_sys("dup2 error");
            close(pipefd[0]);
        }

        execl("/usr/bin/less", "less", (char *)0);
        err_sys("execl error");
    } else {
        close(pipefd[0]);

        while ((n = read(fd, buf, PIPE_BUF)) != 0) {
            if (write(pipefd[1], buf, n) < 0)
                err_sys("write error");
        }

        close(fd);
        close(pipefd[1]);
    }

    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");

    exit(0);
}

