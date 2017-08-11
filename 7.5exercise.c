#include "common.h"

typedef void Exitfunc(void);
static Exitfunc exitfunc;

int main(void)
{
    if (atexit(exitfunc) != 0)
        err_sys("install exit function error");

    exit(0);
}

void exitfunc(void)
{
    printf("Hello\n");
}

