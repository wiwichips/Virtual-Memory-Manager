CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -g

all: virmem

virmem: main.o
	$(CC) main.o -o virmem

main.o: main.c
	$(CC) main.c $(CFLAGS) -c -o main.o

clean:
	rm -rf virmem main.o
