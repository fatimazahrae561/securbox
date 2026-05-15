CC=gcc
CFLAGS=-Wall -pthread

all:
	$(CC) main.c monitor.c logger.c analyzer.c  -o securebox $(CFLAGS)
