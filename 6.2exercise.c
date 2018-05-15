#include "common.h"
#include <shadow.h>

/**
 * 6.1 If the system uses a shadow ﬁle and we need
 * to obtain the encrypted password, how do we do so?
 */
int
main(int argc, char *argv[])
{
    struct spwd *ptr;

    if (argc != 2)
        err_quit("Usage: %s <username>", argv[0]);

    if ((ptr = getspnam(argv[1])) != NULL)
        printf("encrypted password: %s\n", ptr->sp_pwdp);
    else
        err_msg("Unknown user");

    exit(0);
}

