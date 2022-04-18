CC = gcc
CFLAGS = -Wall -g

all: test

compile:
	@$(CC) $(FLAGS) catcher.c -o catcher
	@$(CC) $(FLAGS) sender.c -o sender


run: compile
	@./catcher $(ARGS) &
	@./sender `pidof -s catcher` 1000 $(ARGS)

test: compile
	@echo "\nTESTS FOR 5000 SIGNALS (with signal loss)"
	@echo " mode: kill "
	@./catcher kill &
	@./sender `pidof -s catcher` 1000 kill

	@echo " mode: queue "
	@./catcher queue &
	@./sender `pidof -s catcher` 1000 queue

	@echo " mode: sigrt "
	@./catcher sigrt &
	@./sender `pidof -s catcher` 1000 sigrt

testKill: compile
	@echo " mode: kill "
	@./catcher kill &
	@./sender `pidof -s catcher` 1000 kill


testQueue: compile
	@echo " mode: queue "
	@./catcher queue &
	@./sender `pidof -s catcher` 1000 queue


testSigrt: compile
	@echo " mode: sigrt "
	@./catcher sigrt &
	@./sender `pidof -s catcher` 1000 sigrt


clean:
	rm -f sender catcher catcher.out