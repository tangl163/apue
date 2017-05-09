#include "common.h"

void printde(char *, DIR *dirp);

int main(void)
{
    pid_t pid;
    DIR *dirp;
    struct stat statbuf;

    if (mkdir("test", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0)
        err_sys("mkdir error");

    if (stat("test", &statbuf) < 0)
        err_sys("stat error");

    printf("Before remove: %d\n", statbuf.st_size);

    if ((dirp = opendir("test")) == NULL)
        err_sys("opendir error");

    printde("Before remove", dirp);

    /* fork a child process to execute rmdir. */
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {    // child
        if (rmdir("test") < 0)
            err_sys("rmdir error");

        exit(0);

    } else {
        sleep(3);
    }

    /* Now we wait child to exit. */
    if (waitpid(pid, NULL, 0) != pid)
        err_sys("wait for child error");

    printde("(parent) After remove", dirp);

    if (stat("test", &statbuf) < 0)
        err_sys("stat error");
    printf("After remove: %d\n", statbuf.st_size);

    closedir(dirp);

    exit(0);
}


/**
 * print directory entries.
 */
void printde(char *msg, DIR *dirp)
{
    struct dirent *dir;
    while ((dir = readdir(dirp)) != NULL) {
        printf("%s: %s\n", msg, dir->d_name);
    }
}

