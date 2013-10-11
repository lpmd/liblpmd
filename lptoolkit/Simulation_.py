#
#
#

from lptoolkit.LPMD import Topology, AtomSet, PluginManager
from math import sqrt

Module = lambda v: sqrt(sum(z*z for z in v))

properties = ('step', 'potentialenergy', 'kineticenergy', 'totalenergy', 'temperature', 'linearmomentum')
class Simulation:

    def __init__(self, grid, cell=None, pluginpath=None):
        self.__atomset, self.__cell, self.__top = None, None, None
        self.__grid, self.potentials, self.initialized, self.__step = grid, list(), False, 0
        self.pluginpath = pluginpath
        if cell != None: self.cell = cell

    @property
    def atomset(self): return self.__atomset

    @property
    def grid(self): return self.__grid

    @property
    def topology(self): return self.__top

    @property
    def cell(self): return self.__cell

    @cell.setter
    def cell(self, value):
        self.__cell = value
        if self.__top == None: self.__top = Topology(self.__cell, self.grid)
        if self.__atomset == None: 
           self.__atomset = AtomSet(topology=self.__top, managed=True, useindex=True)

    @property
    def linearmomentum(self): return Module(self.pm['linearmomentum'].Evaluate(self))

    @property
    def temperature(self): return self.pm['temperature'].Evaluate(self)

    @property
    def step(self): return self.__step

    @property
    def standard_properties(self):
        return dict((z, getattr(self, z)) for z in properties)

    def Initialize(self):
        self.pm = PluginManager(self.pluginpath)
        self.pm['centeratoms'].Apply(self, modify=True)
        self.pm['zeroforces'].Apply(self, modify=True)
        self.pm['linearmomentum'].Apply(self, modify=True)
        self.potentialenergy = 0.0
        self.cellmanager.UpdateCell(self, useboundaries=True)
        self.atomset.neighborlist = self.cellmanager.BuildNeighborsList(self)
        for pot in self.potentials: self.potentialenergy += pot.UpdateForces(self, modify=True)
        if hasattr(self.solver, 'Initialize'): self.solver.Initialize(self, modify=True)
        self.initialized = True

    def Run(self, steps):
        if not self.initialized: self.Initialize()
        IsTwoStepIntegrator = lambda s: hasattr(s, 'AdvancePositions') and hasattr(s, 'AdvanceVelocities')
        for self.__step in range(steps):
            if hasattr(self, 'prestep'): self.prestep(self, self.step)
            self.cellmanager.UpdateCell(self, useboundaries=True)
            self.atomset.neighborlist = self.cellmanager.BuildNeighborsList(self)
            self.pm['zeroforces'].Apply(self, modify=True)
            self.potentialenergy = 0.0
            for pot in self.potentials: self.potentialenergy += pot.UpdateForces(self, modify=True)
            self.kineticenergy = 0.0
            if IsTwoStepIntegrator(self.solver):
               self.solver.AdvancePositions(self, modify=True)
               self.cellmanager.UpdateCell(self, useboundaries=True)
               self.atomset.neighborlist = self.cellmanager.BuildNeighborsList(self)
               self.pm['zeroforces'].Apply(self, modify=True)
               for pot in self.potentials: pot.UpdateForces(self, modify=True)
               self.kineticenergy = self.solver.AdvanceVelocities(self, modify=True)
            else: self.kineticenergy = self.solver.Advance(self, modify=True)
            self.totalenergy = self.potentialenergy+self.kineticenergy
            if hasattr(self, 'poststep'): self.poststep(self, self.step)

    def ApplyAtomMask(self, callable):
        callable(self, modify=False, ismask=True)

    def RemoveAtomMask(self): self.atomset.RemoveAtomMask()

