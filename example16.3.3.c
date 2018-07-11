#include "common.h"
#include <netdb.h>

int
main(void)
{
    int i;
    struct hostent *entptr;

    for (i = 0; i < 10; i++) {
        entptr = gethostent();

        if (entptr) {
            printf("%s\n", entptr->h_name);
        }
    }

    endhostent();

    exit(0);
}

