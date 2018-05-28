#include "common.h"
#include <sys/wait.h>

#define POPEN_READ  0
#define POPEN_WRITE 1

static int mypclose(FILE *fp);
static FILE *mypopen(const char *cmdstring, const char *type);

int
main(int argc, char *argv[])
{
    int ret;
    FILE *fp;
    char buf[PIPE_BUF];

    if (argc != 2)
        err_quit("Usage: %s command", argv[0]);

    fp = mypopen(argv[1], "r");
    if (fp == NULL)
        err_sys("mypopen error");

    while (fgets(buf, PIPE_BUF, fp) != NULL) {
        if (fputs(buf, stdout) == EOF)
            err_sys("fputs error");
    }

    if (ferror(fp))
        err_sys("fgets error");

    ret = mypclose(fp);
    if (ret < 0)
        err_sys("mypclose error");

    pr_exit(ret);

    exit(0);
}

static pid_t pid;
static int fd[2];

static FILE *
mypopen(const char *cmdstring, const char *type)
{
    int otype;
    FILE *fp;

    if (cmdstring == NULL || type == NULL
        || (strcmp(type, "r") != 0 && strcmp(type, "w") != 0)) {
        fprintf(stderr, "Invalid arguments\n");
        errno = EINVAL;
        return NULL;
    }

    if (strcmp(type, "r") == 0)
        otype = POPEN_READ;
    else
        otype = POPEN_WRITE;

    if (pipe(fd) < 0) {
        fprintf(stderr, "pipe error\n");
        return NULL;
    }

    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        return NULL;
    } else if (pid == 0) {
        if (otype == POPEN_READ) {
            close(fd[0]);

            if (STDOUT_FILENO != fd[1]) {
                if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
                    err_sys("dup2 error");
                close(fd[1]);
            }
        } else {
            close(fd[1]);

            if (STDIN_FILENO != fd[0]) {
                if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
                    err_sys("dup2 error");
                close(fd[0]);
            }
        }

        execl("/bin/bash", "sh", "-c", cmdstring, (char *)0);
        fprintf(stderr, "execl error\n");
        _exit(127);
    }

    if (otype == POPEN_READ) {
        close(fd[1]);

        fp = fdopen(fd[0], type);
        if (fp == NULL)
            fprintf(stderr, "fdopen error\n");

    } else {
        close(fd[0]);

        fp = fdopen(fd[1], type);
        if (fp == NULL)
            fprintf(stderr, "fdopen error\n");
    }

    return fp;
}

static int
mypclose(FILE *fp)
{
    int status;

    fclose(fp);

    if (waitpid(pid, &status, 0) != pid) {
        fprintf(stderr, "waitpid error\n");
        return -1;
    }

    return status;
}

