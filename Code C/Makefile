CC=gcc
CFLAGS=-Wall 
EXEC=iftun extremite

all: iftun extremite

extremite: extremite.o iftun.o tunalloc.o
	$(CC) -o $@ $^

extremite.o: extremite.c iftun.h tunalloc.h
	$(CC) -o $@ -c $< $(CFLAGS)


iftun: iftun_main.o iftun.o tunalloc.o
	$(CC) -o $@ $^

iftun_main.o: iftun_main.c
	$(CC) -o $@ -c $< $(CFLAGS)

iftun.o: iftun.c iftun.h tunalloc.h
	$(CC) -o $@ -c $< $(CFLAGS)

tunalloc.o: tunalloc.c tunalloc.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o 
	rm extremite
	rm iftun

