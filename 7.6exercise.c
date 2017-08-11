#include "common.h"

#define LEN 10

static void print_i(long *, int);
static void print_ptr(char **, int);

int main(void)
{
    long *ptr;
    char **pc;

    if ((ptr = (long *)calloc(sizeof(long), LEN)) == NULL)
        err_sys("calloc error");
    
    print_i(ptr, LEN);

    if ((pc = (char **)calloc(sizeof(char *), LEN)) == NULL)
        err_sys("calloc error");

    print_ptr(pc, LEN);

    exit(0);
}

static void print_i(long *p, int len)
{
    while (len--)
        printf("%ld ", *p);

    printf("\n");
}

static void print_ptr(char **p, int len)
{
    while (len--)
        printf("%p ", *p);

    printf("\n");
}

