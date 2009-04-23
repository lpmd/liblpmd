#
#
#

all: 
	$(MAKE) test
	./test

test: test.cc vector.h
	g++ -g -O3 -Wall -pedantic -o test test.cc -lm

.PHONY: clean

clean:
	rm -f test *.o

