/* fork, exec, wait 연습 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status;

    printf("부모 pid=%d\n", getpid());
    fflush(stdout);   /* fork 전에 버퍼를 비워야 출력이 중복되지 않는다 */
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        printf("자식 pid=%d, 부모 pid=%d : ls -l 실행\n", getpid(), getppid());
        fflush(stdout);
        execlp("ls", "ls", "-l", "fork_exec.c", NULL);
        perror("execlp");
        exit(2);
    } else {
        wait(&status);
        if (WIFEXITED(status))
            printf("자식(%d) 종료, 종료 코드=%d\n", pid, WEXITSTATUS(status));
    }
    return 0;
}
