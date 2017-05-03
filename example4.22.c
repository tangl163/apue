#include "common.h"

static void myftw(char *);
static void statistic(char *pathname, int length);
static void fcategorise(char *filename, const unsigned int st_mode);

static int n_rfile;  // number of regular file count.
static int n_dfile;  // number of directory count.
static int n_bfile;  // number of block special file count.
static int n_cfile;  // number of character special file count.
static int n_pfile;  // number of named pipe count.
static int n_sfile;  // number of socket count.
static int n_lfile;  // number of symbolic link count.


int main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <pathname>", argv[0]);

    myftw(argv[1]);

    printf("n_rfile: %d\n", n_rfile);
    printf("n_dfile: %d\n", n_dfile);
    printf("n_bfile: %d\n", n_bfile);
    printf("n_cfile: %d\n", n_cfile);
    printf("n_pfile: %d\n", n_pfile);
    printf("n_sfile: %d\n", n_sfile);
    printf("n_lfile: %d\n", n_lfile);
    printf("  total: %d\n", n_rfile + n_dfile + n_bfile + n_cfile + n_pfile + n_sfile + n_lfile);
}


static size_t pathmax;
static char *pathnamebuf;


static void myftw(char *pathname)
{
    pathnamebuf = path_alloc(&pathmax);
    strncpy(pathnamebuf, pathname, pathmax);
    strcat(pathnamebuf, "\0");
    
    statistic(pathnamebuf, strlen(pathnamebuf));
}


/**
 * Recursive dealing file and directory. 
 */
static void statistic(char *pathname, int length)
{
    DIR *dp;
    char *ptr;
    struct stat stbuf;
    struct dirent *dir;
    
    if (lstat(pathname, &stbuf) < 0)
        err_sys("lstat for %s error", pathname);

    if (!S_ISDIR(stbuf.st_mode))
        return;

    if ((dp = opendir(pathname)) == NULL)
        err_sys("opendir path %s error", pathname);

    /* read dir entries */
    while ((dir = readdir(dp)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        ptr = pathname + length + 1;

        *ptr = '\0';

        /* concatenate filename with pathname */
        strncat(pathname, "/", pathmax - length);
        strncat(pathname, dir->d_name, pathmax - length);

        if (lstat(pathname, &stbuf) < 0)
            err_sys("lstat error: %s", pathname);

        fcategorise(pathname, stbuf.st_mode);

        if (S_ISDIR(stbuf.st_mode))
            statistic(pathname, strlen(pathname));
    }

    closedir(dp);
}

/**
 * file type counter.
 */
static void fcategorise(char *filename, const unsigned int st_mode)
{
    if (S_ISREG(st_mode))
        n_rfile++;
    else if (S_ISDIR(st_mode))
        n_dfile++;
    else if (S_ISBLK(st_mode))
        n_bfile++;
    else if (S_ISCHR(st_mode))
        n_cfile++;
    else if (S_ISFIFO(st_mode))
        n_pfile++;
    else if (S_ISSOCK(st_mode))
        n_sfile++;
    else if (S_ISLNK(st_mode))
        n_lfile++;
    else
        fprintf(stderr, "Unknown file %s\n", filename);
}

