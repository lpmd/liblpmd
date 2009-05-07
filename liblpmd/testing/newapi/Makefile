#
#
#

CXXFLAGS=-g -O3 -Wall -ansi -I.. 

all: testparticleset testtask
	./testparticleset
	./testtask

testparticleset: testparticleset.unit particleset.h
	../lpunit/lpmaketest.py testparticleset.unit testparticleset.cc
	g++ $(CXXFLAGS) -o testparticleset testparticleset.cc -L../lpunit -llpunit -lm

testtask: testtask.unit task.h taskrunner.h
	../lpunit/lpmaketest.py testtask.unit testtask.cc
	g++ $(CXXFLAGS) -o testtask testtask.cc -L../lpunit -llpunit -lm

.PHONY: clean

clean:
	rm -f testparticleset testparticleset.cc testtask testtask.cc *.o

