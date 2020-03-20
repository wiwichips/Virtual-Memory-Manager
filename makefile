CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -g

all: main

main: main.c
	$(CC) main.c $(CFLAGS) -o main
