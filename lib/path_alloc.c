#include "../common.h"

#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif

#define PATH_MAX_GUESS 1024    /* max pathname length guess */

/**
 * allocate storage for a pathname.
 */
char *path_alloc(size_t *sizep)
{
    char *ptr;

    if (pathmax == 0) {
        errno = 0;
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
            if (errno == 0)
                pathmax = PATH_MAX_GUESS;
            else
                err_sys("ptehconf error for _PC_PATH_MAX");
        } else {
            pathmax++;
        }
    }

    if ((ptr = malloc(pathmax)) == NULL)
        err_sys("malloc error");

    if (sizep != NULL)
        *sizep = pathmax;
    
    return ptr;
}

