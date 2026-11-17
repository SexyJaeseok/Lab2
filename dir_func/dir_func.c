/* 파일 및 디렉토리 관련 함수 연습: getcwd, mkdir, chdir, stat, opendir/readdir, unlink, rmdir */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main(void)
{
    char buf[512];
    struct stat st;
    DIR *dp;
    struct dirent *de;
    int fd;

    getcwd(buf, sizeof(buf));
    printf("[getcwd] %s\n", buf);

    if (mkdir("testdir", 0755) == -1) {
        perror("mkdir");
        exit(1);
    }
    printf("[mkdir] testdir 생성\n");

    chdir("testdir");
    getcwd(buf, sizeof(buf));
    printf("[chdir] %s\n", buf);

    fd = open("hello.txt", O_WRONLY | O_CREAT, 0644);
    write(fd, "hello linux\n", 12);
    close(fd);
    fd = open("memo.txt", O_WRONLY | O_CREAT, 0600);
    write(fd, "open source project\n", 20);
    close(fd);

    dp = opendir(".");
    printf("[readdir] 현재 디렉토리 목록\n");
    while ((de = readdir(dp)) != NULL) {
        if (stat(de->d_name, &st) == -1)
            continue;
        printf("  %-10s size=%ld mode=%o %s", de->d_name, (long)st.st_size,
               st.st_mode & 0777, ctime(&st.st_mtime));
    }
    closedir(dp);

    unlink("hello.txt");
    unlink("memo.txt");
    chdir("..");
    if (rmdir("testdir") == 0)
        printf("[rmdir] testdir 삭제\n");
    return 0;
}
