#
#
#

CXXFLAGS=-g -O3 -Wall -ansi -I.. 

all: testatom testparticleset 
	./testatom
	./testparticleset

testatom: testatom.unit atom.h
	../lpunit/lpmaketest.py testatom.unit testatom.cc 
	g++ $(CXXFLAGS) -o testatom testatom.cc -L../lpunit -llpunit -lm

testparticleset: testparticleset.unit particleset.h
	../lpunit/lpmaketest.py testparticleset.unit testparticleset.cc
	g++ $(CXXFLAGS) -o testparticleset testparticleset.cc -L../lpunit -llpunit -lm

.PHONY: clean

clean:
	rm -f testatom testatom.cc testparticleset testparticleset.cc *.o

