CC=gcc
CFLAGS=-Wall 
EXEC=iftun

all: iftun extremite

extremite: extremite.o
	$(CC) -o $@ $^

extremite.o: extremite.c 
	$(CC) -o $@ -c $< $(CFLAGS)


iftun: iftun.o tunalloc.o
	$(CC) -o $@ $^

iftun.o: iftun.c tunalloc.h
	$(CC) -o $@ -c $< $(CFLAGS)

tunalloc.o: tunalloc.c tunalloc.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o 
	rm extremite
	rm iftun

