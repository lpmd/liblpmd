#
#
#

CXXFLAGS=-g -O3 -Wall -ansi -I.. 

all: testatom testcell testparticleset 
	./testatom
	./testcell
	./testparticleset

testatom: testatom.unit atom.h
	../lpunit/lpmaketest.py testatom.unit testatom.cc 
	g++ $(CXXFLAGS) -o testatom testatom.cc -L../lpunit -llpunit -lm

testcell: testcell.unit cell.h
	../lpunit/lpmaketest.py testcell.unit testcell.cc
	g++ $(CXXFLAGS) -o testcell testcell.cc -L../lpunit -llpunit -lm

testparticleset: testparticleset.unit particleset.h
	../lpunit/lpmaketest.py testparticleset.unit testparticleset.cc
	g++ $(CXXFLAGS) -o testparticleset testparticleset.cc -L../lpunit -llpunit -lm

.PHONY: clean

clean:
	rm -f testatom testatom.cc testcell testcell.cc testparticleset testparticleset.cc *.o

