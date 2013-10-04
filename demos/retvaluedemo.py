#!/usr/bin/env python3

import sys, builtins, LPMD

nprocs = LPMD.InitializeCommunication(sys.argv)
timer = LPMD.Timer()

sim = LPMD.Simulation(grid=LPMD.AutoGrid(nprocs), cell=LPMD.Cell(a=1.0, b=1.0, c=1.0))

print = lambda *args: (builtins.print(*args) if sim.topology.rank == 0 else None)

pm = LPMD.PluginManager()

a = pm['test']
mat = a.Evaluate(sim)
print (mat)
print (mat.size)
print (mat.GetLabel(0), mat.GetLabel(1), mat.GetLabel(2))
for j in range(mat.size[1]):
    for i in range(mat.size[0]):
        print (mat.Get(i, j))

LPMD.FinishCommunication()

