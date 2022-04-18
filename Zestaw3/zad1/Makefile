CC = gcc
CFLAGS = -Wall -g
ARGS = 5
all: run

compile:
	$(CC) $(FLAGS) main.c -o main

run: compile
	./main $(ARGS)

clean:
	rm -f main


test:
	make --no-print-directory run ARGS=5
	make --no-print-directory run ARGS=25
	make --no-print-directory run ARGS=50
	make --no-print-directory run ARGS=500
	make --no-print-directory run ARGS=1000
