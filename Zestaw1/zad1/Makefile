CC = gcc
CFLAGS = -Wall -g

all: static shared

static:
#	$(CC) $(CFLAGS) -c myLibrary.c -o myLibrary.o
#	ar rcs libmyLibrary.a myLibrary.o
	@gcc -c myLibrary.c
	@ar rcs lib1.a myLibrary.o
	@gcc -c main.c
	@gcc main.o lib1.a -o main

shared:
	@$(CC) $(CFLAGS) -fPIC -c myLibrary.c -o myLibrary.o
	@$(CC) $(CFLAGS) -shared -o libmyLibrary.so myLibrary.o

clean:
	@rm -rf *.o *.a *.so main