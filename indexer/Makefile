CFLAGS=-Wall -pedantic -std=c11 -I../utils/ -L../lib/ -g
LIBS=-lutils -lcurl

indexer:  
			gcc $(CFLAGS) indexer.c $(LIBS) -o $@

clean:
			rm -f indexer
