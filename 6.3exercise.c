#include "common.h"
#include <sys/utsname.h>

/**
 * 6.3 Write a program that calls uname and prints all the ﬁelds
 * in the utsname structure. Comparethe output to the output from
 * the uname(1) command.
 */
int
main(void)
{
    struct utsname name;

    if (uname(&name) < 0)
        err_sys("uname error");

    printf("%s %s %s %s %s\n", name.sysname, name.nodename,
            name.release, name.version, name.machine);

    exit(0);
}

