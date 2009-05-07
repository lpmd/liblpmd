#
#
#

CXXFLAGS=-g -O3 -Wall -ansi -I.. 

all: testparticleset 
	./testparticleset

testparticleset: testparticleset.unit particleset.h
	../lpunit/lpmaketest.py testparticleset.unit testparticleset.cc
	g++ $(CXXFLAGS) -o testparticleset testparticleset.cc -L../lpunit -llpunit -lm

.PHONY: clean

clean:
	rm -f testparticleset testparticleset.cc *.o

