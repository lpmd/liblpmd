#
#
#

TESTS = vectortest utiltest steppertest celltest arraytest

vectortest_SOURCES = vectortest.unit
utiltest_SOURCES = utiltest.unit
steppertest_SOURCES = steppertest.unit
celltest_SOURCES = celltest.unit
arraytest_SOURCES = arraytest.unit

#paramtest_SOURCES = paramtest.unit
#inputfiletest_SOURCES = inputfiletest.unit

#
#
#

check_PROGRAMS = $(TESTS)

AM_CPPFLAGS = -I$(top_src) 
AM_LDFLAGS = ../lpunit/liblpunit.a ../liblpmd.la -ldl -lm

.unit.cc: ../lpunit/liblpunit.a
	../lpunit/lpmaketest.py $< $@

