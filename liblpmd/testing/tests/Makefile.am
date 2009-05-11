#
#
#

TESTS = vectortest utiltest steppertest tasktest orthogonalcelltest nonorthogonalcelltest arraytest atomtest fixedsizeparticlesettest \
        simulationtest simulationcelltest

vectortest_SOURCES = vectortest.unit
utiltest_SOURCES = utiltest.unit
steppertest_SOURCES = steppertest.unit
tasktest_SOURCES = tasktest.unit
orthogonalcelltest_SOURCES = orthogonalcelltest.unit
nonorthogonalcelltest_SOURCES = nonorthogonalcelltest.unit
arraytest_SOURCES = arraytest.unit
atomtest_SOURCES = atomtest.unit
fixedsizeparticlesettest_SOURCES = fixedsizeparticlesettest.unit
simulationtest_SOURCES = simulationtest.unit
simulationcelltest_SOURCES = simulationcelltest.unit

#
#
#

check_PROGRAMS = $(TESTS)

AM_CPPFLAGS = -I$(top_src) 
AM_LDFLAGS = ../lpunit/liblpunit.a ../liblpmd.la -ldl -lm

.unit.cc: ../lpunit/liblpunit.a
	../lpunit/lpmaketest.py $< $@

