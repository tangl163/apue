#include "common.h"

int
main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <pathname>", argv[0]);

    if (faccessat(AT_FDCWD, argv[1], R_OK, AT_EACCESS) < 0)
        err_ret("can't read file %s", argv[1]);
    else
        printf("read access ok\n");

    if (open(argv[1], O_RDONLY) < 0)
        err_ret("open file %s failure", argv[1]);
    else
        printf("read access ok\n");

    exit(0);
}

