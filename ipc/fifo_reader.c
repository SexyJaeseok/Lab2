/* 이름 있는 파이프(FIFO) - 읽는 쪽 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
    int fd, n;
    char buf[100];

    mkfifo("myfifo", 0666);   /* 어느 쪽이 먼저 실행되어도 되도록 둘 다 만든다 */
    fd = open("myfifo", O_RDONLY);
    n = read(fd, buf, sizeof(buf) - 1);
    buf[n] = '\0';
    printf("reader: %s", buf);
    close(fd);
    unlink("myfifo");
    return 0;
}
