#
#
#

TESTS = vectortest utiltest steppertest

vectortest_SOURCES = vectortest.unit
utiltest_SOURCES = utiltest.unit
steppertest_SOURCES = steppertest.unit

#celltest_SOURCES = celltest.unit
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

