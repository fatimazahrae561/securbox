CC=gcc
CFLAGS=-Wall -pthread

all:
	$(CC) main.c monitor.c logger.c analyzer.c stats.c signals.c queue.c log_queue.c -o securebox $(CFLAGS)
