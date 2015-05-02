CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -D_BSD_SOURCE

all: Server Client

Server: server.c
	$(CC) $(CFLAGS) server.c -o HAbwA

Client: heiner.c
	$(CC) $(CFLAGS) heiner.c -o Heiner

clean:
	rm -f *.o
.PHONY: all clean
