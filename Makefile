CC=g++
CFLAGS=-c -Wall -O3

all:	g512sum g256sum

g512sum:	g512sum.o gsumfuncs.o
	$(CC) g512sum.cpp gsumfuncs.cpp -o g512sum

g256sum:	g256sum.o gsumfuncs.o
		$(CC) g256sum.cpp gsumfuncs.cpp -o g256sum

g512sum.o:	g512sum.cpp gsum.h const.h
	$(CC) $(CFLAGS) -c g512sum.cpp

g256sum.o:	g256sum.cpp gsum.h const.h
		$(CC) $(CFLAGS) -c g256sum.cpp

gsumfuncs.o: gsumfuncs.cpp gsum.h const.h
	$(CC) $(CFLAGS) -c gsumfuncs.cpp

clean:
	rm -rf *.o g512sum g256sum
