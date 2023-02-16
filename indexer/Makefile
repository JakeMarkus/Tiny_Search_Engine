CFLAGS=-Wall -pedantic -std=c11 -I../utils/ -L../lib/ -g
LIBS=-lutils -lcurl

all: indexer indexer_step3 indexer_step4

indexer:  
			gcc $(CFLAGS) indexer.c $(LIBS) -o $@

indexer_step3:
			gcc $(CFLAGS) indexer_step3.c $(LIBS) -o $@

indexer_step4:
			gcc $(CFLAGS) indexer_step4.c $(LIBS) -o $@

indexer_step5_part1:
			gcc $(CFLAGS) indexer_step5_part1.c $(LIBS) -o $@
clean:
			rm -f indexer indexer_step3 indexer_step4 indexer_step5_part1
