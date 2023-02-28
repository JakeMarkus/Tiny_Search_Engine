VALGRIND=valgrind --leak-check=full
OFILES=queue.o hash.o webpage.o pageio_new.o indexio.o lqueue.o
CFLAGS=-Wall -pedantic -std=c11 -I. -g

all: $(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

webpage.o: webpage.c webpage.h
	gcc $(CFLAGS) -c $<

pageio_new.o: pageio_new.c pageio.h
	gcc $(CFLAGS) -c $<

indexio.o: indexio.c indexio.h
	gcc $(CFLAGS) -c $<

lqueue.o: lqueue.c lqueue.h
	gcc $(CFLAGS) -c $<

%.v: %
				$(VALGRIND) $<

clean:
	rm -f  lqueue.o webpage.o pageio_new.o indexio.o ../lib/libutils.a
