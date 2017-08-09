#include "common.h"

static void exit_handle(void);
static int explode(char *, char *, char *);

extern char **environ;

/**
 * We use this variable to mark if the environment list has been moved.
 */
static int moved = FALSE;

int main(void)
{
    char buf[MAXLINE];
    char name[MAXLINE];
    char value[MAXLINE];
    char *temp;

    if (atexit(exit_handle) != 0)
        err_sys("call atexit error");

    printf("%%"); 

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        
        if (explode(buf, name, value) < 0)
            continue;

        if ((temp = genenv(name)) != NULL)
            if (modifyenb(temp, name, value) < 0)
                err_msg("An error occurred on changing env");
        else
            if (addenv(name, value) < 0)
                err_msg("An error occurred on adding env");

        exit_handle();

        printf("%%"); 
    }

    if (ferror(stdin))
        err_sys("stdin error");

    exit(0);
}

static int modifyenv(char *env, char *name, char *value)
{
    int len;
    char *p, *pn, *pv;
    char *temp;

    pn = name, pv = value;

    len = strlen(name) + strlen(value) + 2;    // one for `=` and nother for `\0`.

    if (strlen(env) >= len) {
        strcpy(env, name);
        strcat(env, "=");
        strcat(env, value);

    } else {

        if ((temp = envcpy(name, value)) == NULL)
            return -1;

        replaceenv(env, temp);
    }

    return 0;
}

static int addenv(char *name, char *value)
{
    int len;
    char **p;
    char *temp;

    len = envrionlen();
    
    if (moved == FALSE) {
        p = (char **)malloc(sizeof(*environ) * (len + 1));

        if (p == NULL)
            return -1;

        /**
         * if we haven't moved the old environment list, we need to do it.
         */
        copyenvlist(p);

        if ((temp = envcpy(name, value)) == NULL)
            return -1;

        p[len] = temp;
        p[len + 1] = NULL;

    } else {
        p = (char **)realloc(sizeof(*environ) * (len + 1));

        if (p == NULL)
            return -1;

        if ((temp = envcpy(name, value)) == NULL)
            return -1;

        p[len] = temp;
        p[len + 1] = NULL;

        environ = p;
    }

    return 0;
}

static void copyenvlist(char **dest)
{
    char **source;
    char **p;

    source = environ;
    p = dest;

    while (*source)
        *p++ = *source++;
    
    free(environ);

    *p = NULL;
    
    environ = dest;

    /* mark environ as moved */
    moved = TRUE;
}

static char *envcpy(char *name, char *value)
{
    char *p, *pn, *pv;
    char *temp;
    int len;

    pn = name, pv = value;

    len = strlen(name) + strlen(value) + 2;    // one for `=` and nother for `\0`.
    
    if ((p = malloc(sizeof(char) * len)) == NULL)
        return NULL;
    
    temp = p;

    while (*pn)
        *temp++ = *pn++;

    *temp++ = '=';

    while (*pv)
        *temp++ = *pv++;

    *temp = '\0';

    return p;
}

static void replaceenv(char *search, char *replace)
{
    char **p = environ;

    while (*p) {

        if (*p == search) {
            *p = replace;
            free(search);

            break;
        }

        *p++;
    }
}

static int explode(char *source, char *name, char *value)
{
    char *ps, *pn, *pv;
    int c;
    int flag = 1;

    ps = source, pn = name, pv = value;

    while (isspace(*ps))
        ps++;

    if (strlen(ps) == 0)
        return -1;

    while ((c = *ps++) != '\0' && c != '\n') {

        if (isspace(c)) {
            flag = 0;
            continue;
        }

        if (flag)
            *name++ = c;
        else
            *value++ = c;
    }

    *name = *value = 0;
    
    return 0;
}

/**
 * Return the length of environ list including a ternimating `NULL` byte.
 */
static int envrionlen(void)
{
    char **p;
    int count = 1;
    
    p = environ;

    while (*p++)
        count++;

    return count;
}

static void exit_handle(void)
{
    char **p;

    p = environ;

    while (*p)
        printf("%s\n", *p++);
}

