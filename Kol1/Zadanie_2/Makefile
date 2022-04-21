CFLAGS = -Wall
CC = gcc

main: main.o

clean:
	rm -f ./main.o ./main

test:  main
	@echo -e "\n\nTESTING\n\n"
	./main uname hostname
	 @echo -e "============================================================================="
	./main ls ps    
	@echo -e "============================================================================="
	./main who w 
	@echo -e "============================================================================="
	./main abc abc abc
all: main test
