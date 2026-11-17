/* 무명 파이프로 부모-자식 간 통신 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    char buf[100];
    pid_t pid;

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (pid == 0) {
        close(fd[0]);
        char *msg = "자식이 파이프로 보낸 메시지";
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);
        exit(0);
    }
    close(fd[1]);
    read(fd[0], buf, sizeof(buf));
    printf("부모가 받은 메시지: %s\n", buf);
    close(fd[0]);
    wait(NULL);
    return 0;
}
