
SUBDIRS = . lpunit tests demos benchmarks

AM_CPPFLAGS = -g -Wall -std=gnu++98 -DPLUGINPATH="\"$(libdir)/lpmd\""

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblpmd-2.0.pc

lib_LTLIBRARIES = liblpmd.la
liblpmd_la_LDFLAGS = -version-info 2:0:0

nobase_includeHEADERS_INSTALL = $(INSTALL) -D -p -c -m 644

nobase_include_HEADERS =  lpmd/atom.h lpmd/basicvector.h lpmd/basiccell.h lpmd/cell.h lpmd/cellformat.h lpmd/cellgenerator.h lpmd/cellmanager.h \
                          lpmd/cellreader.h lpmd/cellwriter.h lpmd/cmdline.h lpmd/color.h lpmd/containable.h lpmd/coulomb.h lpmd/error.h \
                          lpmd/inputfile.h lpmd/instantproperty.h lpmd/integrator.h lpmd/map.h lpmd/matrix.h lpmd/md.h lpmd/stepper.h \
                          lpmd/metalpotential.h lpmd/module.h lpmd/onestepintegrator.h lpmd/pairpotential.h lpmd/paramlist.h lpmd/particleset.h \
                          lpmd/plugin.h lpmd/pluginmanager.h lpmd/potential.h lpmd/potentialarray.h lpmd/session.h lpmd/simulationcell.h \
                          lpmd/systemmixer.h lpmd/systemmodifier.h lpmd/temporalproperty.h lpmd/timer.h lpmd/twostepintegrator.h lpmd/util.h \
                          lpmd/vector.h lpmd/value.h lpmd/visualizer.h lpmd/neighbor.h lpmd/api.h

noinst_HEADERS = version.h

liblpmd_la_SOURCES = atom.cc cell.cc cellgenerator.cc cellmanager.cc error.cc integrator.cc md.cc module.cc paramlist.cc \
                     particleset.cc plugin.cc pluginmanager.cc potential.cc potentialarray.c pairpotential.cc session.cc \
                     timer.cc onestepintegrator.cc color.cc

#liblpmd_la_SOURCES =  cellformat.cc cellreader.cc cellwriter.cc cmdline.cc coulomb.cc inputfile.cc matrix.cc metalpotential.cc \
#                       systemmixer.cc temporalproperty.cc twostepintegrator.cc visualizer.cc

#
#
#

version.h:
		./update-svnrev.sh

install:
		./update-svnrev.sh
		$(MAKE) install-am

install-exec:
		./update-svnrev.sh
		$(MAKE) install-exec-am

