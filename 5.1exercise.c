#include "common.h"

static void pr_stdio(const char *, FILE *);
static void mysetbuf(FILE *fp, char * const buf);

/**
 * 5.1 Implement setbuf using setvbuf.
 */
int main(void)
{
    char buf[BUFSIZ]; 

    printf("before setbuf\n");
    pr_stdio("stderr", stderr);

    mysetbuf(stderr, buf);

    printf("after setbuf\n");
    pr_stdio("stderr", stderr);

    exit(0);
}

static void mysetbuf(FILE *fp, char *const buf)
{
    int fd;
    int buftype, bufsize;

    if (buf == NULL) {
        buftype = _IONBF;
        bufsize = 0;
        goto setbuf;
    }

    if ((fd = fileno(fp)) < 0)
        err_sys("fileno error");

    if (isatty(fd)) {
        buftype = _IOLBF;
        bufsize = BUFSIZ;

    } else {
        buftype = _IOFBF;
        bufsize = BUFSIZ;
    }

setbuf:

    if (setvbuf(fp, buf, buftype, bufsize) != 0)
        err_sys("setvbuf error");
}

static void pr_stdio(const char *name, FILE *fp)
{
    printf("stream = %s, ", name);

    if (fp->_flags & _IOFBF)
        printf("fully buffered\n");

    else if (fp->_flags & _IOLBF)
        printf("line buffered\n");

    else
        printf("unbuffered\n");
}

