#include "common.h"
#include <sys/time.h>

static void checktime(char *str);

static long long count;
static struct timeval end;

int
main(int argc, char *argv[])
{
    char *str;
    pid_t pid;
    int niceval, adjust_nice;
    
    if (argc != 2)
        err_quit("Usage: %s num", argv[0]);

    errno = 0;
    adjust_nice = strtol(argv[1], NULL, 10);

    if (errno != 0)
        err_sys("strtol error");
    
    gettimeofday(&end, NULL);
    end.tv_sec += 2;
    
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        str = "parent";
        
        errno = 0;
        if ((niceval = nice(0)) == -1 && errno != 0)
            err_sys("nice error");

    } else {
        str = "child";
        
        errno = 0;
        if ((niceval = nice(adjust_nice)) == -1 && errno != 0)
            err_sys("nice error");
    }

    printf("In %s, nice=%d\n", str, niceval);

    while (TRUE) {
        count++;
        checktime(str);
    }
    
    exit(0);
}

static void
checktime(char *str)
{
    struct timeval now;

    gettimeofday(&now, NULL);

    if (now.tv_sec > end.tv_sec && now.tv_usec > end.tv_usec) {
        printf("%s: time out. count=%lld\n", str, count);
        exit(0);
    }
}

