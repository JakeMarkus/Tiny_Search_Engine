VALGRIND=valgrind --leak-check=full
OFILES=queue.o hash.o webpage.o
CFLAGS=-Wall -pedantic -std=c11 -I. -g

all: $(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

webpage.o: webpage.c webpage.h
	gcc $(CFLAGS) -c $<

%.v: %
				$(VALGRIND) $<

clean:
	rm -f  webpage.o ../lib/libutils.a
