#include "common.h"

static void myftw(char *);
static void dopath(void);
static void fcategorise(struct stat *);

static int n_rfile;    // number of regular file count.
static int n_dfile;    // number of directory count.
static int n_bfile;    // number of block special file count.
static int n_cfile;    // number of character special file count.
static int n_pfile;    // number of named pipe count.
static int n_sfile;    // number of socket count.
static int n_lfile;    // number of symbolic link count.
static int n_tfile;    // number of total file count.


int main(int argc, char *argv[])
{
    if (argc != 2)
        err_quit("Usage: %s <pathname>", argv[0]);

    myftw(argv[1]);

    n_tfile = n_rfile + n_dfile + n_bfile + n_cfile + n_pfile + n_sfile + n_lfile;
    if (n_tfile == 0)
        n_tfile = 1;

    printf("n_rfile: %7d \t %7.2f %%\n", n_rfile, n_rfile * 100.0 / n_tfile);
    printf("n_dfile: %7d \t %7.2f %%\n", n_dfile, n_dfile * 100.0 / n_tfile);
    printf("n_bfile: %7d \t %7.2f %%\n", n_bfile, n_bfile * 100.0 / n_tfile);
    printf("n_cfile: %7d \t %7.2f %%\n", n_cfile, n_cfile * 100.0 / n_tfile);
    printf("n_pfile: %7d \t %7.2f %%\n", n_pfile, n_pfile * 100.0 / n_tfile);
    printf("n_sfile: %7d \t %7.2f %%\n", n_sfile, n_sfile * 100.0 / n_tfile);
    printf("n_lfile: %7d \t %7.2f %%\n", n_lfile, n_lfile * 100.0 / n_tfile);
    printf("n_tfile: %7d\n", n_tfile);

    exit(0);
}


static size_t pathlen;
static char *fullpath;


static void myftw(char *pathname)
{
    int actuallen;

    fullpath = path_alloc(&pathlen);
    
    actuallen = strlen(pathname);
    if (pathlen <= actuallen) {
        pathlen =  actuallen * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc error");
    }

    strcpy(fullpath, pathname);
    
    dopath();
}


/**
 * Dealing file and directory recursively. 
 */
static void dopath()
{
    DIR *dp;
    int length;
    struct stat stbuf;
    struct dirent *dir;
    
    if (lstat(fullpath, &stbuf) < 0)
        return err_ret("lstat error for %s", fullpath);

    fcategorise(&stbuf);    // first through dealing the fullpath.

    /* return if not a directory. */
    if (!S_ISDIR(stbuf.st_mode))
        return;

    length = strlen(fullpath);

    /* we add two extra spaces, one for '/' and another for terminating null byte. */
    if (length + NAME_MAX + 2 > pathlen) {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc error");
    }

    if ((dp = opendir(fullpath)) == NULL)
        return err_ret("opendir error directory %s", fullpath);

    fullpath[length++] = '/';
    fullpath[length] = '\0';

    /* read dir entries. */
    while ((dir = readdir(dp)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        /* concatenate filename with fullpath. */
        strcpy(&fullpath[length], dir->d_name);

        if (lstat(fullpath, &stbuf) < 0) {
            err_ret("lstat error: %s", fullpath);
            continue;
        }

        /* add access test. */
        if (S_ISDIR(stbuf.st_mode) && access(fullpath, R_OK | X_OK) == 0)
            dopath();
        else
            fcategorise(&stbuf);
    }

    fullpath[length - 1] = '\0';

    closedir(dp);
}

/**
 * file type counter.
 */
static void fcategorise(struct stat *stbuf)
{
    switch (stbuf->st_mode & S_IFMT) {
        case S_IFREG:
            n_rfile++;
            break;
        case S_IFDIR:
            n_dfile++;
            break;
        case S_IFBLK:
            n_bfile++;
            break;
        case S_IFCHR:
            n_cfile++;
            break;
        case S_IFIFO:
            n_pfile++;
            break;
        case S_IFSOCK:
            n_sfile++;
            break;
        case S_IFLNK:
            n_lfile++;
            break;
        default:
            err_msg("Unknown file: %s", fullpath);
    }
}

