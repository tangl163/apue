#include "common.h"
#include <sys/select.h>
#include <sys/wait.h>

/**
 * 15.7 Explain how select and poll handle an input descriptor that is a pipe,
 * when the pipe is closed by the writer. To determine the answer, write two
 * small test programs: one using select and one using poll.
 *
 * Redo this exercise, looking at an output descriptor that is a pipe, when
 * the read end is closed.
 */
int
main(int argc, char *argv[])
{
    pid_t pid;
    int n, status;
    char mode, buf[PIPE_BUF];
    int maxfd, ready, pipefd[2];
    fd_set readfds, writefds;

    if (argc != 2)
        err_quit("Usage: %s r/w", argv[0]);

    mode = argv[1][0];
    if ((mode != 'w' || mode != 'r') && argv[1][1] != 0)
        err_quit("Invalid arg");

    if (pipe(pipefd) < 0)
        err_sys("pipe error");

    maxfd = MAX(pipefd[0], pipefd[1]);

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);

    pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (mode == 'r') {
            close(pipefd[0]);

            if (pipefd[1] != STDOUT_FILENO) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
        } else {
            close(pipefd[1]);

            if (pipefd[0] != STDIN_FILENO) {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
            }
        }

        execl("/bin/cat", "cat", (char *)0);
        err_sys("excel error");

    } else {
        if (argv[1][0] == 'r') {
            close(pipefd[1]);
            FD_SET(pipefd[0], &readfds);
        } else {
            close(pipefd[0]);
            FD_SET(pipefd[1], &writefds);
        }

        for ( ;; ) {
            ready = select(maxfd + 1, &readfds, &writefds, NULL, NULL);
            if (ready == -1)
                err_sys("select error");

            /* read from pipe */
            if (FD_ISSET(pipefd[0], &readfds)) {
                n = read(pipefd[0], buf, PIPE_BUF);
                if (n == -1)
                    err_sys("read error");

                if (n == 0) {
                    fprintf(stderr, "pipe closed\n");
                    break;
                }

                if (write(STDOUT_FILENO, buf, n) != n)
                    err_sys("write error");
            }

            /* write to pipe */
            if (FD_ISSET(pipefd[1], &writefds)) {
                n = read(STDIN_FILENO, buf, PIPE_BUF);
                if (n == -1)
                    err_sys("read error");

                if (n == 0) {
                    close(pipefd[1]);
                    break;
                }

                if (write(pipefd[1], buf, n) != n)
                    err_sys("write error");
            }
        }
    }

    while (waitpid(pid, &status, 0) != pid) {
        if (errno != EINTR)
            err_sys("waitpid error");
    }

    pr_exit(status);

    exit(0);
}

