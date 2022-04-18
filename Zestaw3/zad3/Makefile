CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	$(CC) $(FLAGS) main.c -o main

run: compile
	./main $(ARGS)

clean:
	rm -f main


tests:
	make --no-print-directory run ARGS=". xyz 4"
	make --no-print-directory run ARGS=". xyz 4"
	make --no-print-directory run ARGS=". xyz 2"
	make --no-print-directory run ARGS=". aa 3"
	make --no-print-directory run ARGS=". bbb 4"
	make --no-print-directory run ARGS=". ab 4"
	make --no-print-directory run ARGS=". a 5"
	make --no-print-directory run ARGS=". cc 2"
	make --no-print-directory run ARGS=". ddd 5"

