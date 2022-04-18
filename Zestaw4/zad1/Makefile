CC = gcc
CFLAGS = -Wall -g


all:
	@echo "\n*************************************************"
	@echo "\n FORK TEST \n"
	@make --no-print-directory testFork
	@echo "\n*************************************************"
	@echo "\n EXEC TEST \n"
	@make --no-print-directory testExec


runFork: compileFork
	./fork $(ARGS)

testFork: compileFork
	@./fork ignore
	@./fork handler
	@./fork mask
	@./fork pending


runExec: compileExec
	./exec $(ARGS)

testExec: compileExec
	@./exec ignore
	@./exec mask
	@./exec pending

compileFork:
	@$(CC) $(FLAGS) fork.c -o fork

compileExec:
	@$(CC) $(FLAGS) exec.c -o exec
	@$(CC) $(FLAGS) childForExec.c -o childForExec

clean:
	rm -f fork exec childForExec