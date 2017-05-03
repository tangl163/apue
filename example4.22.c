#include "common.h"


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
    if (argv != 2)
        err_quit("Usage: %s <pathname>", argv[0]);
}


static 

static void myFtw(char *pathname)
{
    
}

/**
 * file type counter.
 */
static void fcategorise(char *filename, const unsigned int st_mode)
{
    switch (st_mode) {
        case S_ISREG(st_mode):
            n_rfile++;
            break;
        case S_ISDIR(st_mode):
            n_dfile++;
            break;
        case S_ISBLK(st_mode):
            n_bfile++;
            break;
        case S_ISCHR(st_mode):
            n_cfile++;
            break;
        case S_ISFIFO(st_mode):
            n_pfile++;
            break;
        case S_ISSOCK(st_mode):
            n_sfile++;
            break;
        case S_ISLNK(st_mode):
            n_lfile++;
            break;
        default:
            fprintf(stderr, "Unknown file: %s\n", filename);
            break;
    }
}

