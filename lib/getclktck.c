#include "../common.h"
#include <time.h>
#include <sys/times.h>


#ifdef CLK_TCK
static long clktck = CLK_TCK;
#else
static long clktck = 0;
#endif

long getclktck(void)
{
    if (clktck == 0 && (clktck = sysconf(_SC_CLK_TCK) < 0)) {
        err_sys("sysconf error for _SC_CLK_TCK");
    }

    return clktck;
}

