CC = gcc
CFLAGS = -Wall -g
OO = O
R = a


all: static shared

static:
	$(CC) $(CFLAGS) -c myLibrary.c -o myLibrary.o
	ar rcs libmyLibrary.a myLibrary.o



shared:
	$(CC) $(CFLAGS) -fPIC -c myLibrary.c
	$(CC) $(CFLAGS) -shared myLibrary.o -o libmyLibrary.so



clean:
	rm -rf *.o *.a *.so main


cleanRap:
	rm -f raport*.txt

tests:
	rm -f raport3a.txt

	make -s test_static

	make -s test_shared

	make -s test_dynamic


testDiffLevels:
	rm -f raport3b.txt

	@echo "\n Optimization O \n" >> raport3b.txt
	make -s test_static OO=O R=b
	make -s test_shared OO=O R=b
	make -s test_dynamic OO=O R=b

	@echo "\n Optimization Os \n" >> raport3b.txt
	make -s test_static OO=Os R=b
	make -s test_shared OO=Os R=b
	make -s test_dynamic OO=Os R=b

	@echo "\n Optimization O1 \n" >> raport3b.txt
	make -s test_static OO=O1 R=b
	make -s test_shared OO=O1 R=b
	make -s test_dynamic OO=O1 R=b

	@echo "\n Optimization O2 \n" >> raport3b.txt
	make -s test_static OO=O2 R=b
	make -s test_shared OO=O2 R=b
	make -s test_dynamic OO=O2 R=b

	@echo "\n Optimization O3 \n" >> raport3b.txt
	make -s test_static OO=O3 R=b
	make -s test_shared OO=O3 R=b
	make -s test_dynamic OO=O3 R=b

	@echo "\n Optimization O0 \n" >> raport3b.txt
	make -s test_static OO=O0 R=b
	make -s test_shared OO=O0 R=b
	make -s test_dynamic OO=O0 R=b




test_static: static
	$(CC) -c main.c
	$(CC) main.o libmyLibrary.a -o main -${OO}
	@echo "\n STATIC TEST \n" >> raport3${R}.txt
	# --no-print-directory - to not print  “Entering/Leaving directory ...” messages
	make --no-print-directory small >> raport3${R}.txt
	make --no-print-directory medium >> raport3${R}.txt
	make --no-print-directory large >> raport3${R}.txt


test_shared: shared
	$(CC) -c main.c
	$(CC) main.o -L. -lmyLibrary -Wl,-rpath=. -o main -${OO}

	@echo " SHARED TEST " >> raport3${R}.txt

	make --no-print-directory small >> raport3${R}.txt
	make --no-print-directory medium >> raport3${R}.txt
	make --no-print-directory large >> raport3${R}.txt


test_dynamic: shared
	$(CC) main.c -o main -ldl -D DyLIBRARY -${OO}

	@echo " DYNAMIC TEST " >> raport3${R}.txt
	make --no-print-directory small >> raport3${R}.txt
	make --no-print-directory medium >> raport3${R}.txt
	make --no-print-directory large >> raport3${R}.txt





small:
	@echo "\n SMALL TESTS \n"
	@# @ makes silent

	@echo "\n CREATE ARRAY TEST \n"
	@./main createArray 7 getData 7 file1.txt file2.txt file3.txt \
file1.txt file2.txt file3.txt file3.txt file2.txt file3.txt file1.txt

	@echo "\n CREATE AND DELETE TEST \n"
	@./main createArray 10 getData 10 file1.txt file2.txt file3.txt \
    file1.txt file2.txt file3.txt file3.txt file2.txt file3.txt file1.txt \
    deleteBlock 7 deleteBlock 2 deleteBlock 1 deleteBlock 0 deleteBlock 9 \
    deleteBlock 4 deleteBlock 5 deleteBlock 8

	@echo "\n CREATE AND DELETE TEST FEW TIMES \n"
	@./main createArray 10 getData 10 file1.txt file2.txt file3.txt \
	file1.txt file2.txt file3.txt file3.txt file2.txt file3.txt file1.txt \
	deleteBlock 7 deleteBlock 2 deleteBlock 1 deleteBlock 0 deleteBlock 9 \
	deleteBlock 4 deleteBlock 5 deleteBlock 8 deleteBlock 3 deleteBlock 6 \
	createArray 10 getData 10 file1.txt file2.txt file3.txt \
    file1.txt file2.txt file3.txt file3.txt file2.txt file3.txt file1.txt \
    deleteBlock 7 deleteBlock 2 deleteBlock 1 deleteBlock 0 deleteBlock 9 \
    deleteBlock 4 deleteBlock 5 deleteBlock 8


medium:
	@echo "\n MEDIUM TESTS \n"
	@# @ makes silent

	@echo "\n CREATE ARRAY TEST \n"
	@./main createArray 10 getData 10 file4.txt file6.txt file5.txt \
file4.txt file6.txt file5.txt file4.txt file4.txt file6.txt file5.txt

	@echo "\n CREATE AND DELETE TEST \n"
	@./main createArray 10 getData 10 file4.txt file6.txt file5.txt \
file4.txt file6.txt file5.txt file4.txt file4.txt file6.txt file5.txt \
    deleteBlock 7 deleteBlock 2 deleteBlock 1 deleteBlock 0 deleteBlock 9 \
  deleteBlock 4 deleteBlock 5 deleteBlock 8

	@echo "\n CREATE AND DELETE TEST FEW TIMES \n"
	@./main createArray 10 getData 10 file4.txt file6.txt file5.txt \
     file4.txt file6.txt file5.txt file4.txt file4.txt file6.txt file5.txt \
    deleteBlock 7 deleteBlock 2 deleteBlock 1 deleteBlock 0 deleteBlock 9 \
    deleteBlock 4 deleteBlock 5 deleteBlock 8 deleteBlock 3 deleteBlock 6 \
    createArray 10 getData 10 file4.txt file6.txt file5.txt \
    file4.txt file6.txt file5.txt file4.txt file4.txt file6.txt file5.txt \
    deleteBlock 7 deleteBlock 2 deleteBlock 1 deleteBlock 0 deleteBlock 9 \
    deleteBlock 4 deleteBlock 5 deleteBlock 8


large:
	@echo "\n LARGE TESTS \n"
	@# @ makes silent

	@echo "\n CREATE ARRAY TEST \n"
	@./main createArray 10 getData 10 file7.txt file9.txt file8.txt \
file7.txt file9.txt file8.txt file7.txt file7.txt file8.txt file9.txt

	@echo "\n CREATE AND DELETE TEST \n"

	@./main createArray 10 getData 10 file7.txt file9.txt file8.txt \
file7.txt file9.txt file8.txt file7.txt file7.txt file8.txt file9.txt \
deleteBlock 7 deleteBlock 2 deleteBlock 1 deleteBlock 0 deleteBlock 9 \
deleteBlock 4 deleteBlock 5 deleteBlock 8

	@echo "\n CREATE AND DELETE TEST FEW TIMES \n"

	@./main createArray 10 getData 10 file7.txt file9.txt file8.txt \
    file7.txt file9.txt file8.txt file7.txt file7.txt file8.txt file9.txt \
    deleteBlock 7 deleteBlock 2 deleteBlock 1 deleteBlock 0 deleteBlock 9 \
    deleteBlock 4 deleteBlock 5 deleteBlock 8 deleteBlock 3 deleteBlock 6 \
    createArray 10 getData 10 file7.txt file9.txt file8.txt \
    file7.txt file9.txt file8.txt file7.txt file7.txt file8.txt file9.txt \
    deleteBlock 7 deleteBlock 2 deleteBlock 1 deleteBlock 0 deleteBlock 9 \
    deleteBlock 4 deleteBlock 5

