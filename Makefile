CC=gcc                              # překladač jazyka C
CFLAGS=-std=c99 -Wall -Wextra -Werror # parametry překlada
main: main.c
	$(CC) $(CFLAGS) main.c -o main -lm                                  
