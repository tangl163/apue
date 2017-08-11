#include "common.h"

static int f1(int);

int main(int argc, char *argv[])
{
    int val;

    if (argc != 2)
        err_quit("Usage: %s <num>", argv[0]);

    val = atoi(argv[1]);

    printf("%d\n", f1(val));

    exit(0);
}

static int f1(int val)
{
    int num = 0;
    int *ptr = &num;

    if (val == 0) {
        int val;
        val = 5;
        ptr = &val;
    }

    return(*ptr + 1);
}

