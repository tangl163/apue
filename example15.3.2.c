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

static pid_t *fd2pid = NULL;
static long openmax = 0;

static FILE *
mypopen(const char *cmdstring, const char *type)
{
    FILE *fp;
    pid_t pid;
    int i, otype, fd[2];

    if (type == NULL || (type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
        fprintf(stderr, "Invalid arguments\n");
        errno = EINVAL;
        return NULL;
    }

    if (fd2pid == NULL) {
        openmax = open_max();
        fd2pid = calloc(openmax, sizeof(pid_t));
        if (fd2pid == NULL) {
            fprintf(stderr, "malloc error\n");
            return NULL;
        }
    }

    if (type[0] == 'r')
        otype = POPEN_READ;
    else
        otype = POPEN_WRITE;

    if (pipe(fd) < 0) {
        fprintf(stderr, "pipe error\n");
        return NULL;
    }

    if (fd[0] >= openmax || fd[1] >= openmax) {
        fprintf(stderr, "Too many open files\n");
        close(fd[0]);
        close(fd[1]);
        errno = EMFILE;
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

        for (i = 0; i < openmax; i++)
            if (fd2pid[i] > 0)
                close(i);

        execl("/bin/bash", "sh", "-c", cmdstring, (char *)0);
        fprintf(stderr, "execl error\n");
        _exit(127);
    }

    if (otype == POPEN_READ) {
        close(fd[1]);
        fp = fdopen(fd[0], type);
    } else {
        close(fd[0]);
        fp = fdopen(fd[1], type);
    }

    if (fp == NULL)
        fprintf(stderr, "fdopen error\n");
    else
        fd2pid[fileno(fp)] = pid;

    return fp;
}

static int
mypclose(FILE *fp)
{
    int fd;
    pid_t pid;
    int status;

    if (fd2pid == NULL) {
        fprintf(stderr, "popen must be called before calling pclose\n");
        errno = EINVAL;
        return -1;
    }

    fd = fileno(fp);
    if (fd < 0) {
        fprintf(stderr, "filno error\n");
        return -1;
    }

    pid = fd2pid[fd];
    if (pid == 0) {
        fprintf(stderr, "Invalid argument\n");
        errno = EINVAL;
        return -1;
    }

    fclose(fp);
    fd2pid[fd] = 0;

    while (waitpid(pid, &status, 0) < 0) {
        if (errno != EINTR) {
            fprintf(stderr, "waitpid error\n");
            return -1;
        }
    }

    return status;
}

