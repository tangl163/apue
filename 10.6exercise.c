#include "common.h"
#include <signal.h>

#define FILE_PATH "/tmp/test"
#define SIZE 128

static void write_file(char *str);
static void ltostr(long n, char *str);
static void reverse_str(char *str);

static FILE *fp;

/**
 * 10.6 Write the following program to test the parent–child synchronization
 * functions in Figure 10.24. The process creates a file and writes the integer
 * 0 to the file. The process then calls fork, and the parent and child
 * alternate incrementing the counter in the file. Each time the counter is
 * incremented, print which process (parent or child) is doing the increment.
 */

int
main(void)
{
    pid_t pid;

    if ((fp = fopen(FILE_PATH, "w+")) == NULL)
        err_sys("fopen error");

    write_file("init");
    
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {

        write_file("child");
        TELL_PARENT();
        _exit(0);
    } else {

        WAIT_CHILD();
        write_file("parent");
    }

    write_file("done");
    fclose(fp);
    exit(0);
}

static void
write_file(char *str)
{
    long n;
    char buf[SIZE];

    rewind(fp);
    fgets(buf, SIZE, fp);
    ftruncate(fileno(fp), 0);

    if (ferror(fp))
        err_sys("fgets error");

    n = atol(buf);
    n++;

    ltostr(n, buf);

    rewind(fp);
    if (fputs(buf, fp) == EOF)
        err_sys("fputs error");

    fflush(fp);
    printf("%s: %ld\n", str, n);
}

static void
ltostr(long n, char *str)
{
    char *p;
    long i, tmp;

    p = str;
    tmp = n;

    if (n < 0)
        *p++ = '-';

    do {
        i = tmp % 10;
        *p++ = i + '0';
        tmp /= 10;
    } while (tmp);

    *p = 0;

    reverse_str(str);
}

static void
reverse_str(char *str)
{
    char c;
    size_t len, middle, i;

    len = strlen(str);
    middle = len / 2;

    for (i = 0; i < middle; i++) {
        c = str[i];
        str[i] = str[len-1-i];
        str[len-1-i] = c;
    }
}

