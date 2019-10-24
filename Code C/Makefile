CC=gcc
CFLAGS=-Wall 
EXEC=iftun

all: $(EXEC)

$(EXEC): tunalloc.o
	$(CC) -o $@ tunalloc.o

tunalloc.o: tunalloc.c
	$(CC) -o $@ -c $^ $(CFLAGS)

clean:
	rm -f *.o 

