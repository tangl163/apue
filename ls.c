#include "common.h"

int main(int argc, char *argv[])
{
    DIR *dp;
    struct dirent *dir;

    if (argc != 2)
        err_quit("Usage: ls <directory>");

    if ((dp = opendir(argv[1])) == NULL)
        err_sys("opendir: %s error", argv[1]);

    while ((dir = readdir(dp)) != NULL) {
        printf("%s\n", dir->d_name);
    }
    
    closedir(dp);
    exit(0);
}

