/* POSIX 메시지 큐를 이용한 1:1 채팅
 * 사용법: ./mq_chat <보내는 큐> <받는 큐> <이름>   (큐 이름은 /a2b 처럼 / 로 시작)
 * 컴파일: gcc -o mq_chat mq_chat.c -lrt */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MSGSIZE 256

int main(int argc, char *argv[])
{
    mqd_t sq, rq;
    struct mq_attr attr;
    char buf[MSGSIZE + 1], line[200], msg[MSGSIZE + 1];
    pid_t pid;
    ssize_t n;

    if (argc < 4) {
        fprintf(stderr, "사용법: %s <보내는 큐> <받는 큐> <이름>\n", argv[0]);
        return 1;
    }
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSGSIZE;
    attr.mq_curmsgs = 0;

    sq = mq_open(argv[1], O_WRONLY | O_CREAT, 0666, &attr);
    rq = mq_open(argv[2], O_RDONLY | O_CREAT, 0666, &attr);
    if (sq == (mqd_t)-1 || rq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    pid = fork();
    if (pid == 0) {
        /* 자식: 메시지 수신 */
        while ((n = mq_receive(rq, buf, MSGSIZE, NULL)) >= 0) {
            buf[n] = '\0';
            printf("%s", buf);
            fflush(stdout);
            if (strstr(buf, "quit") != NULL)
                break;
        }
        exit(0);
    }

    /* 부모: 입력한 문장을 큐로 전송 */
    while (fgets(line, sizeof(line), stdin) != NULL) {
        snprintf(msg, sizeof(msg), "[%s] %s", argv[3], line);
        mq_send(sq, msg, strlen(msg), 0);
        if (strncmp(line, "quit", 4) == 0)
            break;
    }
    sleep(1);
    kill(pid, SIGTERM);
    wait(NULL);
    mq_close(sq);
    mq_close(rq);
    mq_unlink(argv[1]);
    return 0;
}
