CC = gcc
CFLAGS = -Wall -g
ARGS = a file1.txt

all: run


run:
	$(CC) $(FLAGS) main.c -o main
	make --no-print-directory start >> pomiar_zad_2.txt


start:
	@./main $(ARGS)

clean:
	rm -f main