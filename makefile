CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -g

all: virmem

virmem: virmem.c
	$(CC) virmem.c -o virmem

clean:
	rm -rf *.o

zip: 