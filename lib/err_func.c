#include "common.h"

static void common_output(int errnoflag, int error, const char *format, va_list ap);

/**
 * Print a message then return.
 */
void err_msg(const char *format, ...)
{
    va_list ap;
    int en = errno;
    
    va_start(ap, format);

    common_output(FALSE, en, format, ap);

    va_end(ap);
}


/**
 * Pirnt a message then exit.
 */
void err_quit(const char *format, ...)
{
    va_list ap;
    int en = errno;
    
    va_start(ap, format);

    common_output(FALSE, en, format, ap);

    va_end(ap);

    exit(1);
}

/**
 * Print a message with the error reason phrase then return.
 */
void err_ret(const char *format, ...)
{
    va_list ap;
    int en = errno;

    va_start(ap, format);

    common_output(TRUE, en, format, ap);

    va_end(ap);
}

/**
 * Print a message with the errno reason phrase then exit.
 */
void err_sys(const char *format, ...)
{
    va_list ap;
    int en = errno;

    va_start(ap, format);

    common_output(TRUE, en, format, ap);

    va_end(ap);
    
    exit(1);
}

/**
 * Common output function only could be used internally.
 */
static void common_output(int errnoflag, int error, const char *format, va_list ap)
{
    char buf[MAXLINE];
    int length;

    vsnprintf(buf, MAXLINE, format, ap);

    if (errnoflag) {
        length = strlen(buf);
        snprintf(buf + length, MAXLINE - length, ": %s", strerror(error));
    }

    strcat(buf, "\n");

    fputs(buf, stderr);

    fflush(NULL);
}

