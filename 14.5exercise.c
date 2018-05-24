#include "common.h"
#include <signal.h>
#include <sys/select.h>
#include <time.h>
#include <sys/time.h>

#define SECTOMSEC 1000000

static void sig_int(int signo);
static unsigned int sleep_us(unsigned int microseconds);

/**
 * 14.5 Implement the function sleep_us, which is similar to sleep,
 * but waits for a speciﬁed number of microseconds. Use either select or poll.
 * Compare this function to the BSD usleep function.
 */
int
main(int argc, char *argv[])
{
    struct timespec ts;
    unsigned int sleep, unslept;

    if (argc != 2)
        err_quit("Usage: %s microseconds", argv[0]);

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    sleep = atoi(argv[1]);

    clock_gettime(CLOCK_REALTIME, &ts);
    printf("before sleep: ts.tv_sec: %ld ts.tv_nsec: %ld\n", ts.tv_sec, ts.tv_nsec);

    unslept = sleep_us(sleep);

    clock_gettime(CLOCK_REALTIME, &ts);
    printf("after sleep: ts.tv_sec: %ld ts.tv_nsec: %ld\n", ts.tv_sec, ts.tv_nsec);
    printf("unslept: %u\n", unslept);

    exit(0);
}

static unsigned int
sleep_us(unsigned int microseconds)
{
    struct timeval tv;

    tv.tv_sec = microseconds / SECTOMSEC;
    tv.tv_usec = microseconds % SECTOMSEC;

    if (select(0, NULL, NULL, NULL, &tv) < 0) {
        if (errno == EINTR) {
            return tv.tv_sec * SECTOMSEC + tv.tv_usec;
        } else {
            err_sys("select error");
        }
    }

    return 0;
}

static void
sig_int(int signo)
{
    printf("signo: %d caught\n", signo);
}

