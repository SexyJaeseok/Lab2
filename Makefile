CC = gcc
CFLAGS = -Wall -g

PROGS = dir_func/dir_func ls_R/my_ls_R typing/typing process/fork_exec process/signal_test \
	ipc/pipe_basic ipc/fifo_writer ipc/fifo_reader pipe_chat/pipe_chat mq_chat/mq_chat \
	shm_copy/shm_copy

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

mq_chat/mq_chat: mq_chat/mq_chat.c
	$(CC) $(CFLAGS) -o $@ $< -lrt

shm_copy/shm_copy: shm_copy/shm_copy.c
	$(CC) $(CFLAGS) -o $@ $< -lrt

clean:
	rm -f $(PROGS)

.PHONY: all clean
