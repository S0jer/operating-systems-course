CC = gcc
CFlags = -Wall -g


dynamic:
	$(CC) $(CFLAGS) -fPIC -c bibl1.c
	$(CC) $(CFLAGS) -shared bibl1.o -o libmyLibrary.so
	$(CC) zad1.c -o zad1 -ldl -D DyLIBRARY

run: dynamic
	./zad1


clean:
	rm -rf zad1 *.o *.a *.so