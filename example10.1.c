#include "common.h"
#include <signal.h>

void sig_segv(int signo);

int
main(void)
{
    char *s = "hello";

    signal(SIGSEGV, sig_segv);   

    s[0] = 'i';
    
    exit(0);
}

void sig_segv(int signo)
{
    printf("SIGSEGV: %d caught\n", signo);
    _exit(0);
}

