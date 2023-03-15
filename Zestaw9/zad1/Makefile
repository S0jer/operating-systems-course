CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	$(CC) $(FLAGS) santa.c -o santa -lpthread


run: compile
	./santa $(ARGS)
	make --no-print-directory clean

clean:
	rm -f santa
