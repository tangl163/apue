#include "common.h"
#include <syslog.h>

static int daemonize(char *cmd);

/**
 * 13.1 As we might guess from Figure13.2, when the syslog facility
 * is initialized, either by calling openlog directly or on the ﬁrst
 * call to syslog,the special device ﬁle for the UNIX domain datagram
 * socket, /dev/log,has to be opened. What happens if the user process
 * (the daemon) calls chroot beforecalling openlog?
 */
int
main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s cmd", argv[0]);

    daemonize(argv[1]);

    exit(0);
}

static int
daemonize(char *cmd)
{
    int fd;
    pid_t pid;
    long int openmax;

    umask(0);

    if ((pid = fork()) < 0)
        err_quit("%s: fork error", cmd);
    else if (pid > 0)
        _exit(0);

    setsid();

    if (chdir("/") < 0)
        err_quit("%s: chdir to / error", cmd);

    openmax = open_max();

    for (fd = 0; fd < openmax; fd++)
        close(fd);

    fd = open("/dev/null", O_RDWR);
    if (fd < 0)
        err_quit("%s: open for /dev/null error", cmd);

    if (chroot(cmd) < 0)
        err_quit("%s: chroot error", cmd);

    openlog(cmd, LOG_CONS, LOG_DAEMON);
    syslog(LOG_EMERG, "daemon has been initialized");

    return 0;
}

