
SUBDIRS = . lpunit tests demos

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

liblpmd_la_SOURCES =  atom.cc cell.cc cellformat.cc cellgenerator.cc cellmanager.cc cellreader.cc cellwriter.cc \
                      cmdline.cc color.cc containable.cc coulomb.cc error.cc inputfile.cc instantproperty.cc integrator.cc matrix.cc md.cc \
                      metalpotential.cc module.cc onestepintegrator.cc pairpotential.cc paramlist.cc particleset.cc plugin.cc pluginmanager.cc \
                      potential.cc potentialarray.cc session.cc simulationcell.cc systemmodifier.cc \
                      systemmixer.cc temporalproperty.cc timer.cc twostepintegrator.cc visualizer.cc

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

