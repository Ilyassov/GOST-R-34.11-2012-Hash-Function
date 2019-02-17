CC=g++
CFLAGS=-c -Wall

all: g256sum

g256sum:	g256sum.o g256sumfuncs.o
	$(CC) g256sum.cpp g256sumfuncs.cpp -o g256sum

g256sum.o:	g256sum.cpp g256sum.h const.h
	$(CC) $(CFLAGS) -c g256sum.cpp

g256sumfuncs.o: g256sumfuncs.cpp g256sum.h const.h
	$(CC) $(CFLAGS) -c g256sumfuncs.cpp

clean:
	rm -rf *.o g256sum
