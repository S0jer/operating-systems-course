CFLAGS = -Wall
CC = gcc

main: main.c

clean:
	rm -f main.o main


test: main
	@echo -e "\n\nTesting\n\n"
	./main 120 12
	./main 110 11
	./main 100 10
	./main 90 9

all: test

