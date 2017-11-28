#include "common.h"
#include <signal.h>

#define SIG2STR_MAX 128

#ifndef SIGRTMIN
# define SIGRTMIN  0
# undef  SIGRTMAX
# define SIGRTMAX -1
#endif

#ifndef SIGRTMAX
# define SIGRTMAX -1
# undef  SIGRTMIN
# define SIGRTMIN  0
#endif

#define SIG_NAME(name) {SIG##name, #name}

struct sig_table {
    int signo;
    char *name;
};

static struct sig_table *sig_table_entries = {

#ifdef SIGHUP
    SIG_NAME(HUP),
#endif

#ifdef SIGINT
    SIG_NAME(INT),
#endif

#ifdef SIGQUIT
    SIG_NAME(QUIT),
#endif

#ifdef SIGILL
    SIG_NAME(ILL),
#endif

#ifdef SIGTRAP
    SIG_NAME(TRAP),
#endif

#ifdef SIGABRT
    SIG_NAME(ABRT),
#endif

#ifdef SIGBUS
    SIG_NAME(BUS),
#endif

#ifdef SIGFPE
    SIG_NAME(FPE),
#endif

#ifdef SIGKILL
    SIG_NAME(KILL),
#endif

#ifdef SIGUSR1
    SIG_NAME(USR1),
#endif

#ifdef SIGSEGV
    SIG_NAME(SEGV),
#endif

#ifdef SIGUSR2
    SIG_NAME(USR2),
#endif

#ifdef SIGPIPE
    SIG_NAME(PIPE),
#endif

#ifdef SIGALRM
    SIG_NAME(ALRM),
#endif

#ifdef SIGTERM
    SIG_NAME(TERM),
#endif

#ifdef SIGSTKFLT
    SIG_NAME(STKFLT),
#endif

#ifdef SIGCHLD
    SIG_NAME(CHLD),
#endif

#ifdef SIGCONT
    SIG_NAME(CONT),
#endif

#ifdef SIGSTOP
    SIG_NAME(STOP),
#endif

#ifdef SIGTSTP
    SIG_NAME(TSTP),
#endif

#ifdef SIGTTIN
    SIG_NAME(TTIN),
#endif

#ifdef SIGTTOU
    SIG_NAME(TTOU),
#endif

#ifdef SIGURG
    SIG_NAME(URG),
#endif

#ifdef SIGXCPU
    SIG_NAME(XCPU),
#endif

#ifdef SIGXFSZ
    SIG_NAME(XFSZ),
#endif

#ifdef SIGVTALRM
    SIG_NAME(VTALRM),
#endif

#ifdef SIGPROF
    SIG_NAME(PROF),
#endif

#ifdef SIGWINCH
    SIG_NAME(WINCH),
#endif

#ifdef SIGIO
    SIG_NAME(IO),
#endif

#ifdef SIGPWR
    SIG_NAME(PWR),
#endif

#ifdef SIGSYS
    SIG_NAME(SYS),
#endif

    {0, "EXIT"}
};

static size_t n_sigentries = sizeof(sig_table_entries) / sizeof(sig_table);

static int sig2str(int signo, char *str);
static int str2sig(const char *str, int *signop);

int
main(int argc, char *argv[])
{
    int signo;

    if (argc != 2)
        err_quit("Usage: %s signo", argv[0]);

    signo = atoi(argv[1]);

    printf("%s\n", strsignal(signo));

    exit(0);
}

static int
sig2str(int signo, char *str)
{
    int i;

    for (i = 0; i < n_sigentries; i++) {
        if (sig_table_entries[i].signo == signo) {
            strncpy(str, sig_table_entries[i].name, SIG2STR_MAX);
            return 0;
        }
    }

    if (signo < SIGRTMIN || signo > SIGRTMAX)
        return -1;
}

