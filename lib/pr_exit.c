#include "../common.h"
#include <sys/wait.h>

void pr_exit(int status)
{
    if (WIFEXITED(status))
        printf("Normally exited: %d\n", WEXITSTATUS(status));
    
    else if (WIFSIGNALED(status))
        printf("abnormally exited. signal numbe: %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
                WCOREDUMP(status) ? "(core file generated)" : "");
#else
                "");
#endif
    
    else if (WIFSTOPPED(status))
        printf("child has stopped, signal number: %d\n", WSTOPSIG(status));
}

