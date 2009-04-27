#
#
#

all: 
	$(MAKE) testutil
	./testutil
	$(MAKE) testvector
	./testvector
	$(MAKE) testatom
	./testatom
	$(MAKE) testcell
	./testcell
	$(MAKE) testparticleset
	./testparticleset

testutil: testutil.cc util.h
	g++ -g -O3 -Wall -pedantic -o testutil testutil.cc -lm

testvector: testvector.cc vector3.h vector.h
	g++ -g -O3 -Wall -pedantic -o testvector testvector.cc -lm

testatom: testatom.cc atom.h
	g++ -g -O3 -Wall -pedantic -o testatom testatom.cc -lm

testcell: testcell.cc cell.h
	g++ -g -O3 -Wall -pedantic -o testcell testcell.cc -lm

testparticleset: testparticleset.cc particleset.h
	g++ -g -O3 -Wall -pedantic -o testparticleset testparticleset.cc -lm

.PHONY: clean

clean:
	rm -f testutil testvector testatom testcell testparticleset *.o

