CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	@$(CC) $(FLAGS) main.c -o main -lpthread

run: compile
	./main $(ARGS)

testBrain: compile
	./main 1 numbers img/brain.ascii.pgm neg/brainTest1
	./main 1 block img/brain.ascii.pgm neg/brainTest2
	./main 2 numbers img/brain.ascii.pgm neg/brainTest3
	./main 2 block img/brain.ascii.pgm neg/brainTest4
	./main 4 numbers img/brain.ascii.pgm neg/brainTest5
	./main 4 block img/brain.ascii.pgm neg/brainTest6
	./main 8 numbers img/brain.ascii.pgm neg/brainTest7
	./main 8 block img/brain.ascii.pgm neg/brainTest8
	./main 16 numbers img/brain.ascii.pgm neg/brainTest9
	./main 16 block img/brain.ascii.pgm neg/brainTest10
	./main 99 numbers img/brain.ascii.pgm neg/brainTest11
	./main 99 block img/brain.ascii.pgm neg/brainTest12

testTree: compile
	./main 1 numbers img/fractal_tree.ascii.pgm neg/treeTest1
	./main 1 block img/fractal_tree.ascii.pgm neg/treeTest2
	./main 2 numbers img/fractal_tree.ascii.pgm neg/treeTest3
	./main 2 block img/fractal_tree.ascii.pgm neg/treeTest4
	./main 4 numbers img/fractal_tree.ascii.pgm neg/treeTest5
	./main 4 block img/fractal_tree.ascii.pgm neg/treeTest6
	./main 8 numbers img/fractal_tree.ascii.pgm neg/treeTest7
	./main 8 block img/fractal_tree.ascii.pgm neg/treeTest8
	./main 16 numbers img/fractal_tree.ascii.pgm neg/treeTest9
	./main 16 block img/fractal_tree.ascii.pgm neg/treeTest10
	./main 99 numbers img/fractal_tree.ascii.pgm neg/treeTest11
	./main 99 block img/fractal_tree.ascii.pgm neg/treeTest12

testDla: compile
	./main 1 numbers img/dla.ascii.pgm neg/dlaTest1
	./main 1 block img/dla.ascii.pgm neg/dlaTest2
	./main 2 numbers img/dla.ascii.pgm neg/dlaTest3
	./main 2 block img/dla.ascii.pgm neg/dlaTest4
	./main 4 numbers img/dla.ascii.pgm neg/dlaTest5
	./main 4 block img/dla.ascii.pgm neg/dlaTest6
	./main 8 numbers img/dla.ascii.pgm neg/dlaTest7
	./main 8 block img/dla.ascii.pgm neg/dlaTest8
	./main 16 numbers img/dla.ascii.pgm neg/dlaTest9
	./main 16 block img/dla.ascii.pgm neg/dlaTest10
	./main 99 numbers img/dla.ascii.pgm neg/dlaTest11
	./main 99 block img/dla.ascii.pgm neg/dlaTest12


clean:
	@rm -f main times.txt neg/brainTest* neg/treeTest* neg/dlaTest*

clear:
	@rm -f main
