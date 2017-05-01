#include "./lib/common.h"

int main(int argc, char *argv[])
{
    int i;
    char *ptr;
    struct stat st;

    if (argc == 1)
        err_quit("Usage: %s <file path>", argv[0]);

    for (i = 1; i < argc; i++) {
        ptr = "%s: ", argv[i];
        
        if (lstat(argv[i], &st) < 0) {
            err_sys("lstat error");
            continue;
        }

        if (st.st_mode & S_ISUID)
            printf("uid bit is set\n");
        else
            printf("uid bit is not set\n");

        if (S_ISREG(st.st_mode))
            ptr = "regular file";
        else if (S_ISDIR(st.st_mode))
            ptr = "directory file";
        else if (S_ISCHR(st.st_mode))
            ptr = "character file";
        else if (S_ISBLK(st.st_mode))
            ptr = "block file";
        else if (S_ISFIFO(st.st_mode))
            ptr = "FIFO";
        else if (S_ISLNK(st.st_mode))
            ptr = "symbolic link";
        else if (S_ISSOCK(st.st_mode))
            ptr = "socket";
        else
            ptr = "unknown file";
        
        printf("%s\n", ptr);

        printf("%x\n", st.st_mode);
    }

    exit(0);
}
