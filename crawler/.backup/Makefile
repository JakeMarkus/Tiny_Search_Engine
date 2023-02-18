CFLAGS=-Wall -pedantic -std=c11 -I../utils/ -L../lib/ -g
LIBS=-lutils -lcurl

crawler:  
			gcc $(CFLAGS) crawler.c $(LIBS) -o $@

crawlerstep2:  
			gcc $(CFLAGS) crawlerstep2.c $(LIBS) -o $@

crawlerstep5: crawlerstep5.c
			gcc $(CFLAGS) crawlerstep5.c $(LIBS) -o $@
clean:
			rm -f crawler crawlerstep5
