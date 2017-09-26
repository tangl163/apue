#include "common.h"
#include <signal.h>
#include <sys/wait.h>

void print_exit(siginfo_t *infop);

/**
 * 8.3 Rewrite the program in Figure 8.6 to use waitid instead of wait.
 * Instead of calling pr_exit, determine the equivalent information from the siginfo structure.
 */
int main(void)
{
    pid_t pid;
    int status;
    siginfo_t infop;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        exit(7);

    if (waitid(P_PID, pid, &infop, WEXITED) < 0)
        err_sys("waitid for pid: %d error", pid);

    print_exit(&infop);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        abort();

    if (waitid(P_PID, pid, &infop, WEXITED) < 0)
        err_sys("waitid for pid: %d error", pid);

    print_exit(&infop);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        status /= 0;
    
    if (waitid(P_PID, pid, &infop, WEXITED) < 0)
        err_sys("waitid for pid: %d error", pid);

    print_exit(&infop);
    
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        kill(getpid(), SIGUSR1);

    if (waitid(P_PID, pid, &infop, WEXITED) < 0)
        err_sys("waitid for pid: %d error", pid);

    print_exit(&infop);

    exit(0);
}

void print_exit(siginfo_t *infop)
{
    switch (infop->si_code) {
        case CLD_EXITED:
            printf("normally exited: %d\n", infop->si_status);
            break;

        case CLD_KILLED:
            printf("abnormally exited. signal number: %d\n", infop->si_status);
            break;

        case CLD_DUMPED:
            printf("abnormally exited. signal number: %d (core file generated)\n", infop->si_status);
            break;

        case CLD_STOPPED:
            printf("child has stopped, signal number: %d\n", infop->si_status);
            break;

        default:
            // @TODO
            break;
    }
}

