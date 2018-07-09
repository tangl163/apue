#include "common.h"
#include <poll.h>
#include <sys/wait.h>

#define IGNORE_FD -1

int
main(int argc, char *argv[])
{
    pid_t pid;
    int pipefd[2];
    int len, ready, status;
    char mode, buf[PIPE_BUF];
    struct pollfd fdarray[1];

    if (argc != 2)
        err_quit("Usage: %s r/w", argv[0]);

    mode = argv[1][0];
    if ((mode != 'r' && mode != 'w') || argv[1][1] != 0)
        err_quit("Invalid arg");

    if (pipe(pipefd) < 0)
        err_sys("pipe error");

    pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (mode == 'r') {
            if (pipefd[1] != STDOUT_FILENO) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            close(pipefd[0]);
        } else {
            if (pipefd[0] != STDIN_FILENO) {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
            }
            close(pipefd[1]);
        }

        execl("/bin/cat", "cat", (char *)0);
        err_sys("execl error");
    } else {
        if (mode == 'r') {
            close(pipefd[1]);
            fdarray[0].fd = pipefd[0];
            fdarray[0].events = POLLIN;
        } else {
            close(pipefd[0]);
            fdarray[0].fd = pipefd[1];
            fdarray[0].events = POLLOUT;
        }

        for ( ;; ) {
            ready = poll(fdarray, 1, -1);
            if (ready == -1)
                err_sys("poll error");

            if (fdarray[0].revents & POLLERR)
                err_sys("an error has occurred");

            if (fdarray[0].revents & POLLHUP) {
                fprintf(stderr, "hang up\n");
                break;
            }

            /* read from pipe */
            if (fdarray[0].revents & POLLIN) {
                len = read(fdarray[0].fd, buf, PIPE_BUF);
                if (len == -1)
                    err_sys("read error");

                if (len == 0) {
                    fprintf(stderr, "pipe closed");
                    break;
                }

                if (write(STDOUT_FILENO, buf, len) != len)
                    err_sys("write error");
            }

            /* write to pipe */
            if (fdarray[0].revents & POLLOUT) {
                len = read(STDIN_FILENO, buf, PIPE_BUF);
                if (len == -1)
                    err_sys("read error");

                if (len == 0) {
                    close(fdarray[0].fd);
                    break;
                }

                if (write(fdarray[0].fd, buf, len) != len)
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

