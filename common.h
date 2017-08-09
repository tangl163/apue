/* Common file including the basic header files and functions */

/**
 * Common header.
 */
#ifndef _COMMON_HEADER_
#define _COMMON_HEADER_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/resource.h>

#define MAXLINE 4096    /* max line length */

/**
 * Self-defined type.
 */
enum boolean {
    FALSE,
    TRUE
};

/**
 * Prototypes for our own functions.
 */
void err_msg(const char *, ...);

void err_quit(const char *, ...);

void err_ret(const char *, ...);

void err_sys(const char *, ...);

long open_max(void);

char *path_alloc(size_t *sizep);

void set_fl(int fd, int flag);

void clear_fl(int fd, int flag);

long getclktck(void);

#endif /* _COMMON_HEADER */

