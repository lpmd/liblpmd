#
#
#

VERSION="3.0.0"

TARGETS=libmtwist.a liblpmd.so lpmd_headers config.h liblpmd-3.0.pc plugmaker

CLEANABLE_TARGETS=libmtwist.a liblpmd.so plugmaker



all: $(TARGETS) subdirs


Makefile: packagesetup
	./setup --intel --prefix=/home/jperaltac/local



subdirs:
	cd lptoolkit && $(MAKE)
	cd lpplugmaker && $(MAKE)
	cd binding && $(MAKE)

install: subdirs-install $(TARGETS)
	install -v -d /home/jperaltac/local/lib
	install -v -d /home/jperaltac/local/include/lpmd
	install -v -d /home/jperaltac/local/lib/pkgconfig
	install -v -d /home/jperaltac/local/bin/

	install -v -D liblpmd.so.3.0.0 /home/jperaltac/local/lib
	cp -vd liblpmd.so /home/jperaltac/local/lib
	cp -vd liblpmd.so.3 /home/jperaltac/local/lib
	install -v -D lpmd/atomselection.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/atomset.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/basiccell.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/buffer.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/capi.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/cell.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/celltopology.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/dapi.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/error.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/matrix.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/neighborlist.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/nonorthogonalcell.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/orthogonalcell.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/rawatom.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/region.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/returnvalue.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/selector.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/tagcontainer.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/taskreturnvalue.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/timer.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/topology.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/util.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/vector.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/mtwist.h /home/jperaltac/local/include/lpmd
	install -v -D lpmd/params.h /home/jperaltac/local/include/lpmd
	install -v -D liblpmd-3.0.pc /home/jperaltac/local/lib/pkgconfig
	install -v -D plugmaker /home/jperaltac/local/bin/
	/sbin/ldconfig -n /home/jperaltac/local/lib


uninstall: subdirs-uninstall
	rm -vf /home/jperaltac/local/lib/liblpmd.so.3.0.0
	rm -vf /home/jperaltac/local/lib/liblpmd.so
	rm -vf /home/jperaltac/local/lib/liblpmd.so.3
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/atomselection.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/atomset.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/basiccell.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/buffer.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/capi.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/cell.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/celltopology.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/dapi.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/error.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/matrix.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/neighborlist.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/nonorthogonalcell.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/orthogonalcell.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/rawatom.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/region.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/returnvalue.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/selector.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/tagcontainer.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/taskreturnvalue.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/timer.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/topology.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/util.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/vector.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/mtwist.h
	rm -vf /home/jperaltac/local/include/lpmd/lpmd/params.h
	rm -vf /home/jperaltac/local/lib/pkgconfig/liblpmd-3.0.pc
	rm -vf /home/jperaltac/local/bin/plugmaker
	(rmdir /home/jperaltac/local/lib || true) 2>/dev/null
	(rmdir /home/jperaltac/local/include/lpmd || true) 2>/dev/null
	(rmdir /home/jperaltac/local/lib/pkgconfig || true) 2>/dev/null
	(rmdir /home/jperaltac/local/bin/ || true) 2>/dev/null

mtwist.o: mtwist.c mtwist.h
	/opt/intel/composer_xe_2013.2.146/bin/intel64/icc -Wall -O3 -std=c99 -fast -diag-disable remark -DNDEBUG -fPIC -Dlint -c mtwist.c -o mtwist.o

libmtwist.a: mtwist.o
	ar rcs libmtwist.a mtwist.o

atomset.o: atomset.cc /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/atomset.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/orthogonalcell.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/nonorthogonalcell.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/neighborlist.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/atomselection.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/matrix.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/buffer.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/tagcontainer.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/util.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/timer.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -c atomset.cc -o atomset.o

boundary.o: boundary.cc /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/atomset.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -c boundary.cc -o boundary.o

crossing.o: crossing.cc /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/atomset.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/buffer.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -c crossing.cc -o crossing.o

error.o: error.cc /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/error.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -c error.cc -o error.o

matrix.o: matrix.cc /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/matrix.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/error.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -c matrix.cc -o matrix.o

topology.o: topology.cc /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/vector.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/buffer.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/topology.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/neighborlist.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/atomselection.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/matrix.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/util.h mtwist.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -c topology.cc -o topology.o

vector.o: vector.cc /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/vector.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -c vector.cc -o vector.o

timer.o: timer.cc /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/timer.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -c timer.cc -o timer.o

capi.o: capi.cc /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/atomset.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/basiccell.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/orthogonalcell.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/nonorthogonalcell.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/celltopology.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/matrix.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/atomselection.h params.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -c capi.cc -o capi.o

liblpmd.so: atomset.o boundary.o crossing.o error.o matrix.o topology.o vector.o timer.o capi.o
	mpiCC -parallel -o liblpmd.so.3.0.0 atomset.o boundary.o crossing.o error.o matrix.o topology.o vector.o timer.o capi.o -Wl,-rpath,/home/jperaltac/local/lib -shared -Wl,-soname,liblpmd.so.3 mtwist.o
	rm -f liblpmd.so
	ln -s liblpmd.so.3.0.0 liblpmd.so
	rm -f liblpmd.so.3
	ln -s liblpmd.so.3.0.0 liblpmd.so.3

lpmd_headers: lpmd/atomselection.h lpmd/atomset.h lpmd/basiccell.h lpmd/buffer.h lpmd/capi.h lpmd/cell.h lpmd/celltopology.h lpmd/dapi.h lpmd/error.h lpmd/matrix.h lpmd/neighborlist.h lpmd/nonorthogonalcell.h lpmd/orthogonalcell.h lpmd/rawatom.h lpmd/region.h lpmd/returnvalue.h lpmd/selector.h lpmd/tagcontainer.h lpmd/taskreturnvalue.h lpmd/timer.h lpmd/topology.h lpmd/util.h lpmd/vector.h lpmd/mtwist.h lpmd/params.h


config.h: config.h.in
	cat config.h.in | ./setup --expand="version=3.0.0" > config.h

liblpmd-3.0.pc: liblpmd-3.0.pc.in
	cat liblpmd-3.0.pc.in | ./setup --expand="prefix=/home/jperaltac/local,version=3.0.0" > liblpmd-3.0.pc

plugmaker: plugmaker.py
	cat plugmaker.py > plugmaker


.PHONY: subdirs-install check subdirs-clean clean subdirs-uninstall

subdirs-install: 
	cd lptoolkit && $(MAKE) install
	cd lpplugmaker && $(MAKE) install
	cd binding && $(MAKE) install

check: 


subdirs-clean: 
	cd lptoolkit && $(MAKE) clean
	cd lpplugmaker && $(MAKE) clean
	cd binding && $(MAKE) clean

clean: subdirs-clean
	rm -f $(CLEANABLE_TARGETS) mtwist.o atomset.o boundary.o crossing.o error.o matrix.o topology.o vector.o timer.o capi.o liblpmd.so.3.0.0 liblpmd.so.3

subdirs-uninstall: 
	cd lptoolkit && $(MAKE) uninstall
	cd lpplugmaker && $(MAKE) uninstall
	cd binding && $(MAKE) uninstall

