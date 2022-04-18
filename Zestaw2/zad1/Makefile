CC = gcc
CFLAGS = -Wall -g
ARGS = file1.txt file2.txt

all: run

compile:
	$(CC) $(FLAGS) main.c -o main

run: compile
	./main $(ARGS)

clean:
	rm -f main