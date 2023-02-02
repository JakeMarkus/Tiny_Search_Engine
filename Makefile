CFLAGS=-Wall -pedantic -std=c11 -I. -g
VALGRIND=valgrind --leak-check=full
OFILES=utils/queue.o utils/hash.o utils/webpage.o

all: $(OFILES)
			ar cr lib/libutils.a $(OFILES)

utils/webpage.o: utils/webpage.c
			gcc $(CFLAGS) -c utils/webpage.c -o utils/webpage.o

%.o:	%.c
			gcc $(CFLAGS) -c $<

%.v: %
				$(VALGRIND) $<


clean:
	rm -f
