CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb 
tetris : tetrisRevised.c
	$(CC) $(CFLAGS) -o tetris tetrisRevised.c -lncurses 
