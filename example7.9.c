#include "common.h"

static int envrionlen(void);
static void exit_handle(void);
static char *mygetenv(char *name);
static void copyenvlist(char **dest);
static int explode(char *, char *, char *);
static int addenv(char *name, char *value);
static char *envcpy(char *name, char *value);
static void replaceenv(char *search, char *replace);
static int modifyenv(char *env, char *name, char *value);
static int explode(char *source, char *name, char *value);

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

        if ((temp = mygetenv(name)) != NULL) {
            if (modifyenv(temp, name, value) < 0)
                err_msg("An error occurred on changing env");
        } else {
            if (addenv(name, value) < 0)
                err_msg("An error occurred on adding env");
        }

        printf("%s\n", mygetenv(name));
        //exit_handle();
        //printf("%s\n", environ[20]);

        printf("%%"); 
    }

    if (ferror(stdin))
        err_sys("stdin error");

    exit(0);
}

static char *mygetenv(char *name)
{
    unsigned int i, j;
    unsigned int len;
    char **p = environ;
    char *temp;

    len = strlen(name);

    for (i = 0; p[i] != NULL; i++) {
        temp = p[i];

        for (j = 0; j < len; j++) {
            if (temp[j] != name[j])
                break;
        }

        if (j == len && temp[len] == '=')
            return temp;
    }

    return NULL;
}

static int modifyenv(char *env, char *name, char *value)
{
    unsigned int len;
    char *p;

    len = strlen(name) + strlen(value) + 2;    // one for `=` and nother for `\0`.

    if (strlen(env) >= len) {
        strcpy(env, name);
        strcat(env, "=");
        strcat(env, value);

    } else {

        if ((p = envcpy(name, value)) == NULL)
            return -1;

        replaceenv(env, p);
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
        p = malloc(sizeof *environ * (len + 1));

        if (p == NULL)
            return -1;

        /**
         * if we haven't moved the old environment list, we need to do it.
         */
        copyenvlist(p);

        if ((temp = envcpy(name, value)) == NULL)
            return -1;

        p[len - 1] = temp;
        p[len] = NULL;

    } else {
        p = realloc(environ, sizeof *environ * (len + 1));

        if (p == NULL)
            return -1;

        if ((temp = envcpy(name, value)) == NULL)
            return -1;

        p[len - 1] = temp;
        p[len] = NULL;

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
    
    if ((p = malloc(sizeof *p * len)) == NULL)
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
            //free(search);

            break;
        }

        p++;
    }
}

static int explode(char *source, char *name, char *value)
{
    char *p;
    int c;
    int flag = 1;

    p = source;

    while (isspace(*p))
        p++;

    if (strlen(p) == 0)
        return -1;

    while ((c = *p++) != '\0' && c != '\n') {

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

