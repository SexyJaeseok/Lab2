# lab2

오픈소스프로젝트 실습 2 (20223110 박재석) - 파일 처리, 프로세스 및 프로세스간 통신

실행 환경: Windows 11 WSL2 (Ubuntu), gcc 15.2, GNU Make 4.4, cargo 1.93, Python 3

- matrix_rust : Rust Vec<T> 를 사용한 두 행렬의 덧셈 (실습 순서 2)
- dir_func    : 파일/디렉토리 함수 연습 - getcwd, mkdir, chdir, stat, readdir, unlink, rmdir (3)
- ls_R        : ls -R 과 같은 결과를 내는 재귀 디렉토리 나열 프로그램 (4)
- typing      : 틀린 횟수와 평균 분당 타자수를 재는 타자 연습 (5)
- process     : fork/exec/wait, 시그널(sigaction, kill, alarm) (6)
- ipc         : 무명 파이프, 이름 있는 파이프(FIFO) (7)
- pipe_chat   : 파이프를 이용한 텍스트 채팅 (8)
- mq_chat     : POSIX 메시지 큐를 이용한 텍스트 채팅 (9)
- shm_copy    : POSIX 공유 메모리와 mmap 을 이용한 파일 복사 (10)
- dbus        : DBUS 알림 서버/클라이언트 (11)
- captures    : 실행 결과 화면

팀과제 쉘 프로그램 (12번, 팀 3명)은 이 저장소에 포함되어 있지 않고 별도 저장소에서 진행한다.
https://github.com/SexyJaeseok/mini-shell-team2_1

## 빌드

```
make            # C 프로그램 전체
cd matrix_rust && cargo build
```

## 정리

### 2. Rust 행렬 덧셈 (matrix_rust)
첫 줄에 행과 열 크기를 입력받고, 두 행렬의 원소를 읽어서 `Vec<Vec<i32>>` 에 저장한 뒤 더한다.
C 에서는 malloc 으로 메모리를 잡고 free 로 해제해야 했는데, Vec 은 크기만큼 알아서 할당하고
범위를 벗어나면 자동으로 해제된다. 잘못된 인덱스에 접근하면 panic 이 나서 안전하다.
`cargo run < input.txt` 로 실행한다. (captures/5-2_matrix.png)

### 3. 파일 및 디렉토리 함수 (dir_func)
mkdir 로 디렉토리를 만들고 chdir 로 이동한 뒤 파일 2개를 만들었다. opendir/readdir 로 목록을 읽고
stat 으로 크기와 권한을 출력했고, unlink 와 rmdir 로 정리했다. readdir 은 정렬되지 않은
순서로 읽어 온다는 것을 확인했다. (captures/5-3_dir_func.png)

### 4. ls -R (ls_R)
scandir 로 항목을 정렬해서 읽고, 디렉토리이면 lstat 으로 확인한 뒤 재귀 호출한다.
`LC_ALL=C ls -R` 의 결과와 diff 를 해 보니 차이가 없었다. 숨김 파일은 ls 와 같이 출력하지 않는다.
(captures/5-4_ls_R.png)

### 5. 타자 연습 (typing)
문장 3개를 보여 주고 입력을 받아 글자 단위로 비교해서 틀린 글자 수를 센다.
clock_gettime 으로 입력에 걸린 시간을 재서 (입력 글자 수 x 60 / 시간) 으로 분당 타자수를 계산한다.
길이가 다르면 차이나는 만큼 틀린 것으로 센다. (captures/5-5_typing.png)

### 6. 프로세스와 시그널 (process)
fork 로 자식을 만들고 execlp 로 ls -l 을 실행했다. 부모는 wait 로 종료 상태를 받는다.
시그널은 sigaction 으로 SIGUSR1, SIGALRM 핸들러를 등록하고 자식이 kill 로 SIGUSR1 을 보내도록 했다.
(captures/5-6_fork_exec.png, 5-6_signal.png)

### 7. 프로세스간 통신 (ipc)
pipe 는 부모-자식처럼 관계가 있는 프로세스 사이에서만 쓸 수 있고, mkfifo 로 만든 FIFO 는
이름이 있어서 관계없는 프로세스끼리도 쓸 수 있다. (captures/5-7_pipe_basic.png, 5-7_fifo.png)

### 8. 파이프 채팅 (pipe_chat)
FIFO 를 2개(a2b, b2a) 만들어 한쪽은 쓰기, 다른 쪽은 읽기로 연다. 프로세스를 fork 해서
자식은 상대 메시지를 읽어 출력하고 부모는 키보드 입력을 전송한다. quit 을 입력하면 종료한다.
양쪽이 서로 open 할 때까지 블록되므로 A 와 B 의 open 순서를 다르게 해야 교착 상태가 생기지 않는다.
(captures/5-8_pipe_chat_A.png, 5-8_pipe_chat_B.png)

### 9. 메시지 큐 채팅 (mq_chat)
mq_open, mq_send, mq_receive 를 사용했다. 구조는 pipe_chat 과 같고, 메시지 단위로 전달되기 때문에
read 처럼 일부만 읽는 문제가 없다. 컴파일할 때 `-lrt` 가 필요하다. (captures/5-9_mq_chat_A.png, 5-9_mq_chat_B.png)

### 10. 공유 메모리와 mmap 파일 복사 (shm_copy)
부모가 원본 파일을 mmap 으로 매핑해서 shm_open 으로 만든 공유 메모리에 복사하고,
자식이 공유 메모리를 매핑해서 대상 파일에 쓴다. cmp 로 두 파일이 같은 것을 확인했다.
(captures/5-10_shm_copy.png)

### 11. DBUS 알림 (dbus)
세션 버스에 `kr.lab2.Notifier` 이름으로 서버를 등록하고, 클라이언트가 Notify 메소드를 호출하면
서버가 메시지를 출력하고 응답을 돌려 준다. WSL 에 libdbus 개발 헤더가 없어서 Python 의 dbus-python 으로 구현했다.
dbus-run-session 으로 임시 세션 버스를 띄워서 실행했다. (captures/5-11_dbus.png)

## 검토
- 출력을 파일로 돌리면 fork 전에 버퍼에 남은 내용이 자식에게 복사되어 두 번 나오거나 exec 때 사라지는 문제가 있어서
  fork 전에 fflush 를 했다.
- Windows 드라이브(/mnt/c) 에서는 FIFO 가 제대로 동작하지 않아서 WSL 의 리눅스 파일시스템에서 실행했다.
- 채팅 프로그램은 터미널 두 개에서 실행하는 방식이라, 캡처는 A, B 쪽을 따로 찍었다.
