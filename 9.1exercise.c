#include "common.h"
#include <utmp.h>

int
main(void)
{
    FILE *fp;
    struct utmp tmp;

    if ((fp = fopen("/var/run/utmp", "r")) == NULL)
        err_sys("fopen /var/run/utmp error");

    if (fread(&tmp, sizeof tmp, 1, fp) != 1)
        err_sys("fread error");

    printf("ut_type: %d\n", tmp.ut_type);
    printf("ut_pid: %ld\n", (long)tmp.ut_pid);
    printf("ut_line: %s\n", tmp.ut_line);
    printf("ut_id: %s\n", tmp.ut_id);
    printf("ut_user: %s\n", tmp.ut_user);
    printf("ut_host: %s\n", tmp.ut_host);
    printf("ut_session: %d\n", tmp.ut_session);
    
    exit(0);
}

