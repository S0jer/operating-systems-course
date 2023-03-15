CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	@touch oven
	@touch table
	@$(CC) $(FLAGS) cooking.c -o cooking
	@$(CC) $(FLAGS) supp.c -o supp
	@$(CC) $(FLAGS) main.c -o main

run: compile
	./main $(ARGS)


clean:
	@rm -f main cooking supp oven table


test: compile
	./main 4 4
