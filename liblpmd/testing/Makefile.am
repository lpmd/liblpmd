
SUBDIRS = . lpunit tests demos benchmarks

AM_CPPFLAGS = -g -Wall -std=gnu++98 -DPLUGINPATH="\"$(libdir)/lpmd\""

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblpmd-2.0.pc

lib_LTLIBRARIES = liblpmd.la
liblpmd_la_LDFLAGS = -version-info 2:0:0

nobase_includeHEADERS_INSTALL = $(INSTALL) -D -p -c -m 644

nobase_include_HEADERS =  lpmd/atom.h lpmd/array.h lpmd/basicatom.h lpmd/basiccell.h lpmd/basicparticleset.h \
                          lpmd/cell.h lpmd/cellformat.h lpmd/cellgenerator.h lpmd/cellmanager.h lpmd/cellreader.h lpmd/cellwriter.h lpmd/cmdline.h \
                          lpmd/color.h lpmd/storedvalue.h lpmd/elements.h lpmd/error.h lpmd/properties.h lpmd/orthogonalcell.h \
                          lpmd/nonorthogonalcell.h lpmd/indirectatom.h lpmd/controlfile.h lpmd/property.h lpmd/combinedpotential.h \
                          lpmd/map.h lpmd/matrix.h lpmd/fixedsizeparticleset.h lpmd/twostepintegrator.h lpmd/simulationhistory.h \
                          lpmd/stepper.h lpmd/metalpotential.h lpmd/module.h lpmd/paramlist.h lpmd/solver.h lpmd/storedconfiguration.h \
                          lpmd/particleset.h lpmd/plugin.h lpmd/pluginmanager.h lpmd/potential.h lpmd/pairpotential.h lpmd/session.h \
                          lpmd/configuration.h lpmd/simulation.h lpmd/integrator.h lpmd/onestepintegrator.h lpmd/manipulations.h lpmd/moduleinfo.h \
                          lpmd/systemmixer.h lpmd/systemmodifier.h lpmd/task.h lpmd/taskrunner.h lpmd/timer.h lpmd/simulationengine.h \
                          lpmd/util.h lpmd/vector.h lpmd/value.h lpmd/visualizer.h lpmd/atompair.h lpmd/taghandler.h lpmd/api.h

noinst_HEADERS = version.h

liblpmd_la_SOURCES = cell.cc cellgenerator.cc cellreader.cc cellwriter.cc cellformat.cc cellmanager.cc cmdline.cc controlfile.cc elements.cc \
                     error.cc module.cc metalpotential.cc paramlist.cc plugin.cc pluginmanager.cc potential.cc pairpotential.cc particleset.cc \
                     session.cc matrix.cc simulation.cc timer.cc integrator.cc systemmodifier.cc onestepintegrator.cc twostepintegrator.cc \
                     systemmixer.cc visualizer.cc moduleinfo.cc util.cc

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

