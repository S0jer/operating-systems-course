CC = gcc
CFLAGS = -Wall -g

all: static shared

static:
	$(CC) $(CFLAGS) -c myLibrary.c -o myLibrary.o
	ar rcs libmyLibrary.a myLibrary.o


shared:
	$(CC) $(CFLAGS) -fPIC -c myLibrary.c
	$(CC) $(CFLAGS) -shared myLibrary.o -o libmyLibrary.so


clean:
	rm -rf *.o *.a *.so main

tests:
	rm -f raport2.txt

	make -s test_static


test_static: static
	$(CC) -c main.c
	$(CC) main.o libmyLibrary.a -o main

	@echo "\n STATIC TEST \n" >> raport2.txt
	# --no-print-directory - to not print  “Entering/Leaving directory ...” messages
	make --no-print-directory small >> raport2.txt
	make --no-print-directory medium >> raport2.txt
	make --no-print-directory large >> raport2.txt


test_shared: shared
	$(CC) -c main.c
	$(CC) main.o -L. -lmyLibrary -Wl,-rpath=. -o main -${OO}

	@echo " SHARED TEST " >> raport2.txt

	make --no-print-directory small >> raport2.txt
	make --no-print-directory medium >> raport2.txt
	make --no-print-directory large >> raport2.txt


small:
	@echo "\n SMALL TESTS \n"
	@echo "10 files, 10 - 15 lines \n" >> raport2.txt
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
	@echo "10 files, 30 - 50 lines\n" >> raport2.txt
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
	@echo "10 files, 80 - 100 lines\n" >> raport2.txt
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

