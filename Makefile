override CC = clang-8

ucText: ted.c
	$(CC) ted.c -o ted.o -Wall -Wextra -pedantic -std=c99
clean:
	rm *.o
