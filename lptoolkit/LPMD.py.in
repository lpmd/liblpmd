#
#
#

from sys import version_info
import os, os.path

SearchLibraryPath = 'lib:/usr/lib:/usr/local/lib:/opt/lib'

ok = False
if hasattr(version_info, 'major') and version_info.major == 3: ok = True
elif version_info[0] == 3: ok = True
if not ok: raise SystemError('LPMD module requires Python 3')

import sys, ctypes
sys.path.insert(0, os.path.join('$(prefix)', 'lib'))

from lpmdmodule import AtomSet, Cell, FinishCommunication, InitializeCommunication, NeighborList, Slot, SlotHandler, Timer, Matrix

mpi_lib, lpmd_lib, globaltags = None, None, None

QuickSearchForLibrary = lambda name: [os.path.join(d, name) for d in SearchLibraryPath.split(':') if os.path.exists(os.path.join(d, name))][0]

def __initLPMD__():
    global mpi_lib, lpmd_lib, globaltags
    mpi_lib = ctypes.CDLL(QuickSearchForLibrary('libmpi.so'), ctypes.RTLD_GLOBAL)
    lpmd_lib = ctypes.CDLL(os.path.join(os.path.join('$(prefix)', 'lib'), 'liblpmd.so.3'), ctypes.RTLD_GLOBAL)
    globaltags = dict()

from lptoolkit.Topology import Topology
from lptoolkit.Plugin import Plugin, PluginManager
from lptoolkit.Simulation import Simulation

from lphandler.ControlFile import ControlFile, Statement, UseBlock
from lphandler.QuickMode import QuickMode

def AutoGrid(n):
    from math import fabs, sqrt
    side = int(sqrt(float(n)))
    if n == side*side: return (side, side)
    else:
       divisors = [ z for z in range(1, n+1) if (n % z == 0)]
       pgrids = sorted([ (z, int(n/z)) for z in divisors ], key=lambda w: fabs(w[0]/w[1] - 1.0))
       return pgrids[0] 

__initLPMD__()

from lptoolkit.Tag import Tag

