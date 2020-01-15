CC = gcc -pthread -Wall
PP = (makefile|main.c|text.txt)

all: program

program:
	$(CC)  main.c -o test
clean:
		bash -O extglob -c 'rm -r !$(PP)'
