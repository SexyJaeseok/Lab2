/* 이름 있는 파이프 2개를 이용한 1:1 채팅
 * 사용법: ./pipe_chat <보내는 fifo> <받는 fifo> <내 이름>
 * 한쪽이 ./pipe_chat a2b b2a A, 다른 쪽이 ./pipe_chat b2a a2b B 로 실행한다. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int wfd, rfd;
    char buf[256], msg[300];
    pid_t pid;
    int n;

    if (argc < 4) {
        fprintf(stderr, "사용법: %s <보내는 fifo> <받는 fifo> <이름>\n", argv[0]);
        return 1;
    }
    mkfifo(argv[1], 0666);
    mkfifo(argv[2], 0666);

    /* 양쪽이 서로 열 때까지 기다리므로 순서를 맞춰서 연다 */
    if (strcmp(argv[3], "A") == 0) {
        wfd = open(argv[1], O_WRONLY);
        rfd = open(argv[2], O_RDONLY);
    } else {
        rfd = open(argv[2], O_RDONLY);
        wfd = open(argv[1], O_WRONLY);
    }

    pid = fork();
    if (pid == 0) {
        /* 자식: 상대가 보낸 메시지를 읽어서 출력 */
        while ((n = read(rfd, buf, sizeof(buf) - 1)) > 0) {
            buf[n] = '\0';
            printf("%s", buf);
            fflush(stdout);
            if (strstr(buf, "quit") != NULL)
                break;
        }
        exit(0);
    }

    /* 부모: 키보드 입력을 상대에게 전송 */
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        snprintf(msg, sizeof(msg), "[%s] %s", argv[3], buf);
        write(wfd, msg, strlen(msg));
        if (strncmp(buf, "quit", 4) == 0)
            break;
    }
    sleep(1);
    kill(pid, SIGTERM);
    wait(NULL);
    close(wfd);
    close(rfd);
    return 0;
}
