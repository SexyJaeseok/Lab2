/* ls -R 과 같은 결과를 내는 프로그램 (숨김 파일은 ls 처럼 출력하지 않는다) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

static int is_first = 1;

/* scandir 에서 쓰는 정렬 함수 (LC_ALL=C 의 ls 와 같은 순서) */
static int cmp(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

static int not_hidden(const struct dirent *d)
{
    return d->d_name[0] != '.';
}

void list_dir(const char *path)
{
    struct dirent **names;
    struct stat st;
    char sub[4096];
    int n, i;

    n = scandir(path, &names, not_hidden, cmp);
    if (n < 0) {
        perror(path);
        return;
    }

    if (!is_first)
        printf("\n");
    is_first = 0;
    printf("%s:\n", path);
    for (i = 0; i < n; i++)
        printf("%s\n", names[i]->d_name);

    /* 디렉토리이면 재귀적으로 방문 */
    for (i = 0; i < n; i++) {
        snprintf(sub, sizeof(sub), "%s/%s", path, names[i]->d_name);
        if (lstat(sub, &st) == 0 && S_ISDIR(st.st_mode))
            list_dir(sub);
    }

    for (i = 0; i < n; i++)
        free(names[i]);
    free(names);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "사용법: %s 디렉토리\n", argv[0]);
        return 1;
    }
    list_dir(argv[1]);
    return 0;
}
