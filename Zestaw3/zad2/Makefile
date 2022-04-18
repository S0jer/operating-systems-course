CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	$(CC) $(FLAGS) main.c -o main

run: compile
	./main $(ARGS)

clean:
	rm -f main w*.txt

cleanRap:
	rm -f raport.txt


tests: compile
	@echo "\n TEST 1: 0.002 25 " >> raport.txt
	make -s test1 >> raport.txt

	@echo "\n TEST 2: 0.002 500 " >> raport.txt
	make -s test2 >> raport.txt

	@echo "\n TEST 3 0.002 1000 " >> raport.txt
	make -s test3 >> raport.txt

	@echo "\n TEST 4 0.00002 25 " >> raport.txt
	make -s test4 >> raport.txt

	@echo "\n TEST 5 0.00002 500 " >> raport.txt
	make -s test5 >> raport.txt

	@echo "\n TEST 6 0.00002 1000 " >> raport.txt
	make -s test6 >> raport.txt

	@echo "\n TEST 7 0.00002 2000 " >> raport.txt
	make -s test7 >> raport.txt

test1:
	./main 0.002 25


test2:
	./main 0.002 500


test3:
	./main 0.002 1000


test4:
	./main 0.00002 25


test5:
	./main 0.00002 500


test6:
	./main 0.00002 1000


test7:
	./main 0.0000002 2000


