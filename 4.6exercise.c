#include "common.h"

#define IN TRUE    /* inside file hole */
#define OUT FALSE  /* outside file hole */

#define BUFFSIZE 4096
#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

static int write2fd(int fd, char *buf, int nbytes);
static int llseek(int fd, int offset, int whence);

/**
 * 4.6. Write a utility like cp(1) that copies a ﬁle containing holes,
 * without writing the bytes of 0 to the output ﬁle.
 */
int
main(int argc, char *argv[])
{
    int n, i, j;
    int fd1, fd2;
    int flag, offset;
    char rdbuf[BUFFSIZE];
    char wrbuf[BUFFSIZE];
    
    if (argc != 3)
        err_quit("Usage: %s <source file> <dest file>", argv[0]);
    
    if ((fd1 = open(argv[1], O_RDONLY)) < 0)
        err_sys("open %s error", argv[1]);
    
    if ((fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, RWRWRW)) < 0)
        err_sys("open %s error", argv[2]);

    while ((n = read(fd1, rdbuf, BUFFSIZE)) > 0) {
        flag = OUT;    // first we are out of hole.
        offset = 0;    // initial file hole offset.

        for (i = 0, j = 0; i < n; i++) {
            if (rdbuf[i] != '\0') {
                if (flag == OUT && offset > 0 && llseek(fd2, offset, SEEK_CUR))
                    offset = 0;    // reset offset.

                flag = IN;
                wrbuf[j++] = rdbuf[i];
            } else {
                if (flag == IN && j > 0 && write2fd(fd2, wrbuf, j))
                    j = 0;

                flag = OUT;
                offset++;
            }
        }

        /* we make sure this utility to work properly if the `source file` has no hole. */
        if (flag == IN && j > 0)
            write2fd(fd2, wrbuf, j);
    }

    close(fd1);
    close(fd2);
    
    exit(0);
}


/**
 * Another version of `write`. It's intended to avoid nesting too deep of controlling clause.
 */
static int
write2fd(int fd, char *buf, int nbytes)
{
    if (write(fd, buf, nbytes) != nbytes) {
        err_sys("write error");
    }

    return TRUE;
}

/**
 * Another version of `lseek`. It's intended to avoid nesting too deep of controlling clause.
 */
static int
llseek(int fd, int offset, int whence)
{
    if (lseek(fd, offset, whence) == -1)
        err_sys("lseek error");

    return TRUE;
}

