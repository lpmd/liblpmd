#
#
#

include ../Makefile.params

SONAME_VERSION = 1
LIB_VERSION = 1.0.1

OBJECTS = util.o timer.o vector.o matrix.o paramlist.o cmdline.o inputfile.o module.o plugin.o pluginmanager.o atom.o particles.o cell.o simulationcell.o cellgenerator.o systemmodifier.o visualizer.o potential.o potentialarray.o pairpotential.o coulomb.o integrator.o onestepintegrator.o twostepintegrator.o md.o applicable.o containable.o scalarvalue.o vectorvalue.o scalartable.o instantproperty.o temporalproperty.o basicproperties.o cellwriter.o cellreader.o cellformat.o

all: liblpmd.so

liblpmd.so: $(OBJECTS)
	$(CXX) -shared -Wl,-soname,liblpmd.so.$(SONAME_VERSION) -o liblpmd.so.$(LIB_VERSION) $(LDFLAGS) $(OBJECTS)
	rm -f liblpmd.so.$(SONAME_VERSION) liblpmd.so
	ln -s liblpmd.so.$(LIB_VERSION) liblpmd.so.$(SONAME_VERSION)
	ln -s liblpmd.so.$(SONAME_VERSION) liblpmd.so

defaultpath.h: defaultpath.h.in
	sed "s:REPLACE_BY_DEFAULT_PATH:$(PWD)/../plugins:g" defaultpath.h.in > defaultpath.h

pluginmanager.o: pluginmanager.cc lpmd/pluginmanager.h defaultpath.h
	$(CXX)  -c $(CXXFLAGS) -I. -fPIC $< -o $@

%.o: %.cc lpmd/%.h 
	$(CXX)  -c $(CXXFLAGS) -I. -fPIC $< -o $@

.PHONY: clean

clean:
	-rm -rf $(OBJECTS) defaultpath.h liblpmd.so.* liblpmd.so *~

