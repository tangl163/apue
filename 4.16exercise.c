#include "common.h"

#define FILE_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
#define MAXSIZE 4096 * 20

/**
 * 4.16 Does the UNIX System have a fundamental limitation on the depth of a directory tree? To
 * ﬁnd out, write a program that creates a directory and then changes to that directory, in a loop. 
 */
int
main(int argc, char *argv[])
{
    char *p;
    size_t i, size;
    int depth;

    if (argc != 3)
        err_quit("Usage: %s <dirname> <depth>", argv[0]);

    depth = atoi(argv[2]);

    p = path_alloc(&size);

    for (i = 0; i < depth; i++) {
        if (mkdir(argv[1], FILE_MODE) < 0 || chdir(argv[1]) < 0)
            err_sys("an error occurs");
    }

    while (getcwd(p, size) == NULL) {
        size += 100;
        if (size > MAXSIZE)
            err_quit("giving up");

        if ((p = realloc(p, size)) == NULL)
            err_sys("realloc error");
    }

    printf("path length: %ld\n%s\n", strlen(p), p);

    free(p);

    exit(0);
}

