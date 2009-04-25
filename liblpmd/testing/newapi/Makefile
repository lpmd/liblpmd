#
#
#

all: 
	$(MAKE) testvector
	./testvector
	$(MAKE) testatom
	./testatom
	$(MAKE) testcell
	./testcell

testvector: testvector.cc vector.h
	g++ -g -O3 -Wall -pedantic -o testvector testvector.cc -lm

testatom: testatom.cc atom.h
	g++ -g -O3 -Wall -pedantic -o testatom testatom.cc -lm

testcell: testcell.cc cell.h
	g++ -g -O3 -Wall -pedantic -o testcell testcell.cc -lm

.PHONY: clean

clean:
	rm -f testvector testatom testcell *.o

