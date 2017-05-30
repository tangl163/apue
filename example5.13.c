#include "common.h"

void make_temp(char *);

int main(void)
{
    char template1[] = "/tmp/tempfile1XXXXXX";
    char *template2 = "/tmp/tempfile2XXXXXX";

    printf("creating file %1$s: %1$p ...\n", template1);
    make_temp(template1);
    
    printf("creating file %1$s: %1$p ...\n", template2);
    make_temp(template2);

    exit(0);
}

void make_temp(char *template)
{
    int fd;
    struct stat statbuf;

    if ((fd = mkstemp(template)) < 0)
        err_sys("mkstemp error");

    if (stat(template, &statbuf) < 0)
        err_ret("stat error");

    printf("file exists: %s\n", template);
    close(fd);
    unlink(template);
}

