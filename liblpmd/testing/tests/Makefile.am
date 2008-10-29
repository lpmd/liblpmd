#
#
#

TESTS = celltest paramtest vectortest

celltest_SOURCES = celltest.unit
paramtest_SOURCES = paramtest.unit
vectortest_SOURCES = vectortest.unit

#
#
#

check_PROGRAMS = $(TESTS)

AM_CPPFLAGS = -I$(top_src) 
AM_LDFLAGS = ../lpunit/liblpunit.a ../liblpmd.la -ldl -lm

.unit.cc:
	../lpunit/lpmaketest.py $< $@

