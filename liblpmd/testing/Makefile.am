
SUBDIRS = . lpunit tests

AM_CPPFLAGS = -g -Wall -pedantic -DPLUGINPATH="\"$(libdir)/lpmd\""

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblpmd-1.0.pc

lib_LTLIBRARIES = liblpmd.la
liblpmd_la_LDFLAGS = -version-info 1:0:0

nobase_include_HEADERS =  lpmd/applicable.h lpmd/atom.h lpmd/cell.h lpmd/cellformat.h lpmd/cellgenerator.h lpmd/cellmanager.h \
                          lpmd/cellreader.h lpmd/cellwriter.h lpmd/cmdline.h lpmd/color.h lpmd/containable.h lpmd/coulomb.h lpmd/error.h lpmd/inputfile.h \
                          lpmd/instantproperty.h lpmd/integrator.h lpmd/matrix.h lpmd/md.h lpmd/metalpotential.h lpmd/module.h \
                          lpmd/onestepintegrator.h lpmd/pairpotential.h lpmd/paramlist.h lpmd/particles.h lpmd/physunits.h lpmd/plugin.h \
                          lpmd/pluginmanager.h lpmd/potential.h lpmd/potentialarray.h lpmd/scalartable.h lpmd/scalarvalue.h lpmd/simulationcell.h \
                          lpmd/systemmodifier.h lpmd/temporalproperty.h lpmd/timer.h lpmd/twostepintegrator.h lpmd/util.h lpmd/vector.h \
                          lpmd/vectorvalue.h lpmd/visualizer.h lpmd/neighbor.h lpmd/api.h

noinst_HEADERS = distcache.h

liblpmd_la_SOURCES =  applicable.cc atom.cc cell.cc cellformat.cc cellgenerator.cc cellmanager.cc cellreader.cc cellwriter.cc \
                      cmdline.cc color.cc containable.cc coulomb.cc distcache.cc error.cc inputfile.cc instantproperty.cc integrator.cc matrix.cc md.cc \
                      metalpotential.cc module.cc onestepintegrator.cc pairpotential.cc paramlist.cc particles.cc plugin.cc pluginmanager.cc \
                      potential.cc potentialarray.cc replicamatrix.cc scalartable.cc scalarvalue.cc simulationcell.cc systemmodifier.cc \
                      temporalproperty.cc timer.cc twostepintegrator.cc util.cc vectorvalue.cc visualizer.cc

