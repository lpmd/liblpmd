#
#
#

TESTS = testplugin neighborlist simplemd nopluginmd

testplugin_SOURCES = testplugin.cc 
neighborlist_SOURCES = neighborlist.cc 
simplemd_SOURCES = simplemd.cc 
nopluginmd_SOURCES = nopluginmd.cc 

#
#
#

check_PROGRAMS = $(TESTS)

AM_CPPFLAGS = -I$(top_src) 
AM_LDFLAGS = ../liblpmd.la -ldl -lm

