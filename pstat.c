#include "./lib/common.h"


int main(int argc, char *argv[])
{
    struct stat st;

    if (argc != 2)
        err_quit("Usage %s <filename>", argv[0]);

    if (stat(argv[1], &st) < 0)
        err_sys("stat error");

    printf("st_mode: %o\n", st.st_mode);
    printf("st_ino: %ld\n", st.st_ino);
    printf("st_dev: %ld\n", st.st_dev);
    printf("st_rdev: %ld\n", st.st_rdev);
    printf("st_nlink: %ld\n", st.st_nlink);
    printf("st_uid: %d\n", st.st_uid);
    printf("st_gid: %d\n", st.st_gid);
    printf("st_size: %ld\n", st.st_size);
    printf("st_blksize: %ld\n", st.st_blksize);
    printf("st_blocks: %ld\n", st.st_blocks);
    printf("st_atim.tv_sec: %ld\n", st.st_atim.tv_sec);
    printf("st_atim.tv_nsec: %ld\n", st.st_atim.tv_nsec);
    printf("st_mtim.tv_sec: %ld\n", st.st_mtim.tv_sec);
    printf("st_mtim.tv_nsec: %ld\n", st.st_mtim.tv_nsec);
    printf("st_ctim.tv_sec: %ld\n", st.st_ctim.tv_sec);
    printf("st_ctim.tv_nsec: %ld\n", st.st_ctim.tv_nsec);

    exit(0);
}

