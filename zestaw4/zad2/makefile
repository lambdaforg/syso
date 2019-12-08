CC = gcc -Wall
PP = (makefile|main.c|child.c)

all: program

program:
	$(CC)  main.c -o test
	$(CC)  child.c -o program

	
clean:
		bash -O extglob -c 'rm -r !$(PP)'
