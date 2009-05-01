#
#
#

CXXFLAGS=-g -O3 -Wall -pedantic -I.. 
all: 
	$(MAKE) testatom
	./testatom
	$(MAKE) testcell
	./testcell
	$(MAKE) testparticleset
	./testparticleset

testatom: testatom.cc atom.h
	g++ $(CXXFLAGS) -o testatom testatom.cc -lm

testcell: testcell.cc cell.h
	g++ $(CXXFLAGS) -o testcell testcell.cc -lm

testparticleset: testparticleset.cc particleset.h
	g++ $(CXXFLAGS) -o testparticleset testparticleset.cc -lm

.PHONY: clean

clean:
	rm -f testatom testcell testparticleset *.o

