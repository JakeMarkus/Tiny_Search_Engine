VALGRIND=valgrind --leak-check=full
OFILES=queue.o hash.o webpage.o pageio_new.o
CFLAGS=-Wall -pedantic -std=c11 -I. -g

all: $(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

webpage.o: webpage.c webpage.h
	gcc $(CFLAGS) -c $<

pageio_new.o: pageio_new.c pageio.h
	gcc $(CFLAGS) -c $<

%.v: %
				$(VALGRIND) $<

clean:
	rm -f  webpage.o pageio_new.o ../lib/libutils.a
