CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -g

all: virmem

virmem: virmem.c
	$(CC) $(CFLAGS) virmem.c -o virmem

clean:
	rm -rf *.o virmem

zip:
	zip -r CIS3110_a3_1056636.zip virmem.c answers-a3.txt makefile dataStructurePageTable.h dataStructureTLB.h virmem.h