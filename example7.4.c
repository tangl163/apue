#include "common.h"

extern char **environ;

int main(void)
{
    char **p;
    
    p = environ;

    while (*p)
        printf("%s\n", *p++);

    exit(0);
}

