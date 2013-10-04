#
#
#

VERSION="3.0.0"

TARGETS=lpmdmodule.so

CLEANABLE_TARGETS=lpmdmodule.so



all: $(TARGETS)

install: $(TARGETS)
	install -v -d /home/jperaltac/local/lib

	install -v -D lpmdmodule.so /home/jperaltac/local/lib


uninstall:
	rm -vf /home/jperaltac/local/lib/lpmdmodule.so
	(rmdir /home/jperaltac/local/lib || true) 2>/dev/null

py_cell.o: py_cell.cc /usr/include/python3.2mu/Python.h /usr/include/python3.2mu/structmember.h py_cell.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c py_cell.cc -o py_cell.o

py_topology.o: py_topology.cc /usr/include/python3.2mu/Python.h /usr/include/python3.2mu/structmember.h py_topology.h py_cell.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c py_topology.cc -o py_topology.o

py_atomset.o: py_atomset.cc /usr/include/python3.2mu/Python.h py_topology.h py_atomset.h py_neighborlist.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/error.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c py_atomset.cc -o py_atomset.o

py_neighborlist.o: py_neighborlist.cc /usr/include/python3.2mu/Python.h /usr/include/python3.2mu/structmember.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/neighborlist.h py_neighborlist.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c py_neighborlist.cc -o py_neighborlist.o

py_atomselection.o: py_atomselection.cc /usr/include/python3.2mu/Python.h /usr/include/python3.2mu/structmember.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/atomselection.h py_atomselection.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c py_atomselection.cc -o py_atomselection.o

py_slothandler.o: py_slothandler.cc /usr/include/python3.2mu/Python.h /usr/include/python3.2mu/structmember.h py_slothandler.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c py_slothandler.cc -o py_slothandler.o

py_slot.o: py_slot.cc /usr/include/python3.2mu/Python.h /usr/include/python3.2mu/structmember.h py_slot.h py_atomset.h py_cell.h py_neighborlist.h py_matrix.h py_atomselection.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/atomset.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/basiccell.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/matrix.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c py_slot.cc -o py_slot.o

py_matrix.o: py_matrix.cc /usr/include/python3.2mu/Python.h /usr/include/python3.2mu/structmember.h /home/jperaltac/Projects/lpmd/07/liblpmd/lpmd/matrix.h py_matrix.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c py_matrix.cc -o py_matrix.o

py_timer.o: py_timer.cc /usr/include/python3.2mu/Python.h /usr/include/python3.2mu/structmember.h py_timer.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c py_timer.cc -o py_timer.o

lpmdmodule.o: lpmdmodule.cc /usr/include/python3.2mu/Python.h py_cell.h py_topology.h py_atomset.h py_neighborlist.h py_slothandler.h py_slot.h py_timer.h py_matrix.h py_atomselection.h
	/usr/local/bin/mpiCC -Wall -O3 -axSSE4.1 -parallel -diag-disable remark -finline-functions -DNDEBUG  -fPIC -I. -fPIC -shared  -fPIC -I/usr/include/python3.2mu -I/usr/include/python3.2mu -I.. -c lpmdmodule.cc -o lpmdmodule.o

lpmdmodule.so: py_cell.o py_topology.o py_atomset.o py_neighborlist.o py_atomselection.o py_slothandler.o py_slot.o py_matrix.o py_timer.o lpmdmodule.o ../liblpmd.so
	mpiCC -parallel -o lpmdmodule.so py_cell.o py_topology.o py_atomset.o py_neighborlist.o py_atomselection.o py_slothandler.o py_slot.o py_matrix.o py_timer.o lpmdmodule.o -Wl,-rpath,/home/jperaltac/local/lib -fPIC -shared  ../liblpmd.so -L/usr/lib/python3.2/config-3.2mu -lpthread -ldl -lutil -lm -lpython3.2mu -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions


.PHONY: check clean

check: 


clean: 
	rm -f $(CLEANABLE_TARGETS) py_cell.o py_topology.o py_atomset.o py_neighborlist.o py_atomselection.o py_slothandler.o py_slot.o py_matrix.o py_timer.o lpmdmodule.o

