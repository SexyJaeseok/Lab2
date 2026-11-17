/* mmap 과 POSIX 공유 메모리로 파일 복사
 * 부모: 원본 파일을 mmap 으로 매핑해서 공유 메모리에 복사
 * 자식: 공유 메모리를 mmap 으로 매핑해서 대상 파일에 기록
 * 컴파일: gcc -o shm_copy shm_copy.c -lrt */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHM_NAME "/copy_shm"

int main(int argc, char *argv[])
{
    int sfd, shm_fd, dfd;
    struct stat st;
    char *src, *shm, *dst;
    pid_t pid;

    if (argc < 3) {
        fprintf(stderr, "사용법: %s 원본파일 대상파일\n", argv[0]);
        return 1;
    }

    sfd = open(argv[1], O_RDONLY);
    if (sfd == -1 || fstat(sfd, &st) == -1) {
        perror(argv[1]);
        return 1;
    }
    src = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, sfd, 0);

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, st.st_size);
    shm = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    memcpy(shm, src, st.st_size);
    printf("부모: %s (%ld바이트) 를 공유 메모리에 복사\n", argv[1], (long)st.st_size);

    fflush(stdout);
    pid = fork();
    if (pid == 0) {
        dfd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
        ftruncate(dfd, st.st_size);
        dst = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, dfd, 0);
        memcpy(dst, shm, st.st_size);
        msync(dst, st.st_size, MS_SYNC);
        munmap(dst, st.st_size);
        close(dfd);
        printf("자식: 공유 메모리의 내용을 %s 에 기록\n", argv[2]);
        exit(0);
    }
    wait(NULL);

    munmap(src, st.st_size);
    munmap(shm, st.st_size);
    close(sfd);
    close(shm_fd);
    shm_unlink(SHM_NAME);
    return 0;
}
