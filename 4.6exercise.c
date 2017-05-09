#include "common.h"

#define BUFFSIZE 4096
#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

/**
 * 4.6. Write a utility like cp(1) that copies a ﬁle containing holes,
 * without writing the bytes of 0 to the output ﬁle.
 */
int main(int argc, char *argv[])
{
    int n, i, j;
    int fd1, fd2;
    char rdbuf[BUFFSIZE];
    char wrbuf[BUFFSIZE];
    
    if (argc != 3)
        err_quit("Usage: %s <source file> <dest file>", argv[0]);
    
    if ((fd1 = open(argv[1], O_RDONLY)) < 0)
        err_sys("open %s error", argv[1]);
    
    if ((fd2 = open(argc[2], O_RDWR | O_CREAT | O_TRUNC, RWRWRW)) < 0)
        err_sys("open %s error", argv[2]);

    while ((n = read(fd1, rdbuf, BUFFSIZE)) > 0) {
        for (i = 0, j = 0; i < n; i++) {
            if (rdbuf[i] != '\0') 
                wrbuf[j++] = rdbuf[i];
            else
                ;    // # Todo
        }
    }
}

