#include "common.h"
#include <pthread.h>
#include <signal.h>

#define EXISTS_PREV_ALARMER  0x001
#define SLEEP_LT_PREV        0x010
#define SLEEP_GT_PREV        0x100

static unsigned int sleep1(unsigned int seconds);

/**
 * 12.6 Reimplement the program in Figure10.29 to make it thread-safe
 * without using nanosleep or clock_nanosleep.
 */
int
main(int argc, char *argv[])
{
    unsigned int seconds, unslept;

    if (argc != 2)
        err_quit("Usage: %s seconds", argv[0]);

    seconds = atoi(argv[1]);

    alarm(10);
    unslept = sleep1(seconds);

    printf("unslept: %u\n", unslept);
    printf("left alarm: %u\n", alarm(0));

    exit(0);
}

static unsigned int
sleep1(unsigned int seconds)
{
    int signo, flag;
    unsigned int remainder, unslept;
    sigset_t set, oldset;

    signo = flag = 0;

    sigemptyset(&set);
    sigaddset(&set, SIGALRM);

    pthread_sigmask(SIG_BLOCK, &set, &oldset);

    remainder = alarm(0);
    if (remainder != 0) {
        flag |= EXISTS_PREV_ALARMER;

        /* a previous alarmer exists and its number of seconds less than the argument */
        if (remainder < seconds) {
            flag |= SLEEP_GT_PREV;
            alarm(remainder);
        } else {
            flag |= SLEEP_LT_PREV;
            alarm(seconds);
        }
    } else {
        alarm(seconds);
    }

    sigwait(&set, &signo);
    unslept = alarm(0);

    if (flag & EXISTS_PREV_ALARMER) {
        if (flag & SLEEP_GT_PREV) {
            alarm(unslept);                          // reset if being awaked by signals other than SIGALRM
            unslept = seconds - remainder + unslept;
        } else {
            alarm(remainder - seconds + unslept);    // reset previous alarmer
        }
    }

    return unslept;
}

