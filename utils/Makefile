VALGRIND=valgrind --leak-check=full
OFILES=queue.o hash.o webpage.o

all: $(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

webpage.o: webpage.c
			gcc $(CFLAGS) -c webpage.c -o webpage.o

%.o:	%.c
			gcc $(CFLAGS) -c $<

%.v: %
				$(VALGRIND) $<
clean:
	rm -f  webpage.o ../lib/libutils.a
