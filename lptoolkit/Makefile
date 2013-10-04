#
#
#

VERSION="3.0.0"

TARGETS=__init__.py LPMD.py Plugin.py Simulation.py Tag.py Topology.py

CLEANABLE_TARGETS=



all: $(TARGETS)

install: $(TARGETS)
	install -v -d /home/jperaltac/local/lib/lptoolkit

	install -v -D __init__.py /home/jperaltac/local/lib/lptoolkit
	install -v -D LPMD.py /home/jperaltac/local/lib/lptoolkit
	install -v -D Plugin.py /home/jperaltac/local/lib/lptoolkit
	install -v -D Simulation.py /home/jperaltac/local/lib/lptoolkit
	install -v -D Tag.py /home/jperaltac/local/lib/lptoolkit
	install -v -D Topology.py /home/jperaltac/local/lib/lptoolkit


uninstall:
	rm -vf /home/jperaltac/local/lib/lptoolkit/__init__.py
	rm -vf /home/jperaltac/local/lib/lptoolkit/LPMD.py
	rm -vf /home/jperaltac/local/lib/lptoolkit/Plugin.py
	rm -vf /home/jperaltac/local/lib/lptoolkit/Simulation.py
	rm -vf /home/jperaltac/local/lib/lptoolkit/Tag.py
	rm -vf /home/jperaltac/local/lib/lptoolkit/Topology.py
	(rmdir /home/jperaltac/local/lib/lptoolkit || true) 2>/dev/null

__init__.py: 


LPMD.py: LPMD.py.in
	cat LPMD.py.in | ../setup --expand="prefix=/home/jperaltac/local" > LPMD.py

Plugin.py: 


Simulation.py: 


Tag.py: 


Topology.py: 



.PHONY: check clean

check: 


clean: 
	rm -f $(CLEANABLE_TARGETS) 

