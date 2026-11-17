/* 타자 연습 프로그램: 틀린 글자 수와 분당 타자수(타/분)를 측정한다 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NSENT 3

char *sentences[NSENT] = {
    "Linux is a free and open source operating system.",
    "The quick brown fox jumps over the lazy dog.",
    "A process is a program in execution."
};

int main(void)
{
    char input[256];
    int i, j, len, typed = 0, wrong = 0;
    struct timespec t1, t2;
    double sec, total = 0;

    printf("=== 타자 연습 (문장 %d개) ===\n", NSENT);
    for (i = 0; i < NSENT; i++) {
        printf("\n[%d] %s\n    ", i + 1, sentences[i]);
        fflush(stdout);

        clock_gettime(CLOCK_MONOTONIC, &t1);
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;
        clock_gettime(CLOCK_MONOTONIC, &t2);
        input[strcspn(input, "\n")] = '\0';

        sec = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9;
        total += sec;

        /* 글자 단위로 비교해서 틀린 개수를 센다 (길이가 다르면 차이만큼 틀린 것으로 본다) */
        len = strlen(sentences[i]);
        for (j = 0; j < len; j++) {
            if (input[j] != sentences[i][j])
                wrong++;
        }
        if ((int)strlen(input) > len)
            wrong += strlen(input) - len;
        typed += strlen(input);
    }

    printf("\n=== 결과 ===\n");
    printf("총 입력 글자 수 : %d\n", typed);
    printf("틀린 글자 수    : %d\n", wrong);
    printf("걸린 시간       : %.1f초\n", total);
    if (total > 0)
        printf("평균 분당 타자수: %.0f타/분\n", typed * 60.0 / total);
    return 0;
}
