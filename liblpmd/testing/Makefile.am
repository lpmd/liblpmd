
SUBDIRS = . lpunit tests demos benchmarks

AM_CPPFLAGS = -g -Wall -std=gnu++98 -DPLUGINPATH="\"$(libdir)/lpmd\""

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblpmd-2.0.pc

lib_LTLIBRARIES = liblpmd.la
liblpmd_la_LDFLAGS = -version-info 2:0:0

nobase_includeHEADERS_INSTALL = $(INSTALL) -D -p -c -m 644

nobase_include_HEADERS =  lpmd/atom.h lpmd/array.h lpmd/arraytest.h lpmd/atominterface.h lpmd/basiccell.h lpmd/basicparticleset.h \
                          lpmd/cell.h lpmd/cellformat.h lpmd/cellgenerator.h lpmd/cellmanager.h lpmd/cellreader.h lpmd/cellwriter.h lpmd/cmdline.h \
                          lpmd/color.h lpmd/containable.h lpmd/coulomb.h lpmd/elements.h lpmd/error.h lpmd/properties.h lpmd/orthogonalcell.h \
                          lpmd/nonorthogonalcell.h lpmd/indirectatom.h lpmd/inputfile.h lpmd/instantproperty.h \
                          lpmd/map.h lpmd/matrix.h lpmd/fixedsizeparticleset.h \
                          lpmd/stepper.h lpmd/metalpotential.h lpmd/module.h lpmd/paramlist.h \
                          lpmd/particleset.h lpmd/plugin.h lpmd/pluginmanager.h lpmd/potential.h lpmd/pairpotential.h lpmd/session.h \
                          lpmd/simulationcell.h lpmd/simulation.h lpmd/integrator.h lpmd/onestepintegrator.h \
                          lpmd/systemmixer.h lpmd/systemmodifier.h lpmd/temporalproperty.h lpmd/task.h lpmd/taskrunner.h lpmd/timer.h \
                          lpmd/util.h lpmd/vector.h lpmd/value.h lpmd/visualizer.h lpmd/neighbor.h lpmd/api.h

noinst_HEADERS = version.h

liblpmd_la_SOURCES = cell.cc cellgenerator.cc cellmanager.cc elements.cc error.cc module.cc paramlist.cc \
                     plugin.cc pluginmanager.cc potential.cc pairpotential.cc particleset.cc session.cc \
                     timer.cc integrator.cc onestepintegrator.cc

#liblpmd_la_SOURCES =  cellformat.cc cellreader.cc cellwriter.cc cmdline.cc coulomb.cc inputfile.cc matrix.cc metalpotential.cc \
#                       systemmixer.cc temporalproperty.cc visualizer.cc

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

