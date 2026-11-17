/* 시그널 연습: sigaction, kill, alarm */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t got_usr1 = 0;
static volatile sig_atomic_t got_alarm = 0;

void usr1_handler(int signo)
{
    got_usr1++;
}

void alarm_handler(int signo)
{
    got_alarm = 1;
}

int main(void)
{
    struct sigaction sa;
    pid_t pid;

    sa.sa_handler = usr1_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    sa.sa_handler = alarm_handler;
    sigaction(SIGALRM, &sa, NULL);

    pid = fork();
    if (pid == 0) {
        /* 자식: 부모에게 SIGUSR1 을 두 번 보낸다 */
        sleep(1);
        kill(getppid(), SIGUSR1);
        sleep(1);
        kill(getppid(), SIGUSR1);
        exit(0);
    }

    printf("부모(%d): SIGUSR1 기다리는 중...\n", getpid());
    while (got_usr1 < 2) {
        pause();
        printf("부모: SIGUSR1 수신 (%d번째)\n", got_usr1);
    }
    wait(NULL);

    alarm(2);
    printf("부모: alarm(2) 설정\n");
    while (!got_alarm)
        pause();
    printf("부모: SIGALRM 수신, 종료\n");
    return 0;
}
