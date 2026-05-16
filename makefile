CC=gcc
CFLAGS=-Wall -pthread

all:
	$(CC) main.c monitor.c logger.c analyzer.c stats.c signals.c -o securebox $(CFLAGS)
