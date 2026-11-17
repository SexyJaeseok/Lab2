/* 이름 있는 파이프(FIFO) - 쓰는 쪽 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
    int fd;
    char *msg = "FIFO 를 통해 전달된 메시지\n";

    mkfifo("myfifo", 0666);
    fd = open("myfifo", O_WRONLY);
    write(fd, msg, strlen(msg));
    close(fd);
    printf("writer: 메시지 전송 완료\n");
    return 0;
}
