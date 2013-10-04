#
#
#

import pickle
from lpmdmodule import Topology as innerTopology

class Topology(innerTopology):

    def GatherFromAll(self, rank0, object):
        gat = innerTopology.GatherFromAll(self, rank0, pickle.dumps(object))
        if gat == None: return tuple()
        return tuple(pickle.loads(z) for z in gat)

