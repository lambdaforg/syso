CC = gcc -Wall -pthread 
PP = (makefile|barber.c|client.c|info.h)

all: program

program:
	$(CC)  barber.c -o barber -lrt
	$(CC)  client.c -o client -lrt
clean:
		bash -O extglob -c 'rm -r !$(PP)'
