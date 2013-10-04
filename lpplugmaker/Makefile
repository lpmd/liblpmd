#
#
#

VERSION="3.0.0"

TARGETS=PluginProcessor.py CParser.py CFormatter.py ShuntingYard.py __init__.py

CLEANABLE_TARGETS=



all: $(TARGETS)

install: $(TARGETS)
	install -v -d /home/jperaltac/local/lib/lpplugmaker

	install -v -D PluginProcessor.py /home/jperaltac/local/lib/lpplugmaker
	install -v -D CParser.py /home/jperaltac/local/lib/lpplugmaker
	install -v -D CFormatter.py /home/jperaltac/local/lib/lpplugmaker
	install -v -D ShuntingYard.py /home/jperaltac/local/lib/lpplugmaker
	install -v -D __init__.py /home/jperaltac/local/lib/lpplugmaker


uninstall:
	rm -vf /home/jperaltac/local/lib/lpplugmaker/PluginProcessor.py
	rm -vf /home/jperaltac/local/lib/lpplugmaker/CParser.py
	rm -vf /home/jperaltac/local/lib/lpplugmaker/CFormatter.py
	rm -vf /home/jperaltac/local/lib/lpplugmaker/ShuntingYard.py
	rm -vf /home/jperaltac/local/lib/lpplugmaker/__init__.py
	(rmdir /home/jperaltac/local/lib/lpplugmaker || true) 2>/dev/null

PluginProcessor.py: 


CParser.py: 


CFormatter.py: 


ShuntingYard.py: 


__init__.py: 



.PHONY: check clean

check: 


clean: 
	rm -f $(CLEANABLE_TARGETS) 

