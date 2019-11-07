CC=gcc
CFLAGS=-Wall 
EXEC=iftun extremite tunnel46

all: iftun extremite tunnel46

tunnel46: tunnel46.o iftun.o extremite.o tunalloc.o
	$(CC)  -o $@ $^ -lpthread

tunnel46.o: tunnel46.c iftun.h extremite.h tunalloc.h
	$(CC) -o $@ -c $< $(CFLAGS)

extremite: extremite_main.o extremite.o iftun.o tunalloc.o
	$(CC)  -o $@ $^ -lpthread

extremite.o: extremite.c iftun.h tunalloc.h
	$(CC) -o $@ -c $< $(CFLAGS)

extremite_main.o: extremite_main.c 
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
	rm tunnel46

