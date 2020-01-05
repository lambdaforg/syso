CC = gcc -Wall
PP = (makefile|barber.c|client.c|info.h)

all: program

program:
	$(CC)  barber.c -o barber
	$(CC)  client.c -o client
clean:
		bash -O extglob -c 'rm -r !$(PP)'
