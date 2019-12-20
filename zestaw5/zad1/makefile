CC = gcc -Wall
PP = (makefile|server.c|main.c|info.h)

all: program

program:
	$(CC)  server.c -o server
	$(CC)  main.c -o test
clean:
		bash -O extglob -c 'rm -r !$(PP)'
