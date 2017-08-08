#include "common.h"

static void exit_handler1(void);
static void exit_handler2(void);

int main(void)
{
    if (atexit(exit_handler1) != 0)
        err_sys("atexit for exit_handler1 error");

    if (atexit(exit_handler2) != 0)
        err_sys("atexit for exit_handler2 error");

    _exit(0);
}

static void exit_handler1(void)
{
    printf("exit_handler1\n");
}

static void exit_handler2(void)
{
    printf("exit_handler2\n");
}

