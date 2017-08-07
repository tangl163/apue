#include "common.h"
#include <netdb.h>

#define STAY_OPEN 1

int main(void)
{
    struct hostent *ptr;

    sethostent(STAY_OPEN);

    while ((ptr = gethostent()) != NULL)
        printf("hostname: %s\n", ptr->h_name);

    endhostent();
    exit(0);
}

