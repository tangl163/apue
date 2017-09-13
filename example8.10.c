#include "common.h"

/**
 * When a process with super privileges `exec` a `setuid`
 * executable (doesn't belong to `root`), it will lose the super priveleges.
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <filenam>", argv[0]);

    if (setuid(0) < 0)
        err_sys("setuid error");

    printf("uid=%ld, gid=%ld, euid=%ld, egid=%ld\n",
            (long)getuid(), (long)getgid, (long)geteuid(), (long)getegid());

    if (unlink(argv[1]) < 0)
        err_sys("unlink error");

    exit(0);
}

