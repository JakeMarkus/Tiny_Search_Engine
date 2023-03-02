CFLAGS=-Wall -pedantic -std=c11 -I../utils/ -L../lib/ -g
LIBS=-lutils -lcurl

all: test1 test_indexsave testlqueue testlqueuem

test1:  
			gcc $(CFLAGS) test1.c $(LIBS) -o $@

test_indexsave:
			gcc $(CFLAGS) test_indexsave.c $(LIBS) -o $

testlqueue:
			gcc $(CFLAGS) testlqueue.c $(LIBS) -o $@

testlqueuem:
			gcc $(CFLAGS) testlqueuem.c $(LIBS) -o $@

clean:
			rm -f test1 test_indexsave testlqueue testlqueuem
