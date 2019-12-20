CC = gcc -Wall 
PP = (makefile|server.c|main.c|info.h)

all: program

program:
	$(CC)  server.c -o server -lrt
	$(CC)  main.c -o test -lrt
clean:
		bash -O extglob -c 'rm -r !$(PP)'
