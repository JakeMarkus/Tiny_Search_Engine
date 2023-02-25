CFLAGS=-Wall -pedantic -std=c11 -I../utils/ -L../lib/ -g                     
LIBS=-lutils -lcurl

all: query

query:
			gcc $(CFLAGS) query.c $(LIBS) -o $@

querystep2:
			gcc $(CFLAGS) querystep2.c $(LIBS) -o $@
clean:
			rm -f query
