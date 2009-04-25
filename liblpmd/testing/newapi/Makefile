#
#
#

all: 
	$(MAKE) test
	./test
	$(MAKE) testatom
	./testatom
	$(MAKE) testcell
	./testcell

test: test.cc vector.h
	g++ -g -O3 -Wall -pedantic -o test test.cc -lm

testatom: testatom.cc atom.h
	g++ -g -O3 -Wall -pedantic -o testatom testatom.cc -lm

testcell: testcell.cc cell.h
	g++ -g -O3 -Wall -pedantic -o testcell testcell.cc -lm

.PHONY: clean

clean:
	rm -f test testatom testcell *.o

