#!/usr/bin/env python3

import sys, builtins, LPMD

nx, ny, nz = 10, 10, 10
nc, cutoff, bl = 12, 4.2, 8.5

nprocs = LPMD.InitializeCommunication(sys.argv)
timer = LPMD.Timer()

cmname = (sys.argv[1] if len(sys.argv) > 1 else 'minimumimage')
grid = LPMD.AutoGrid(nprocs)
sim = LPMD.Simulation(grid=grid, cell=LPMD.Cell(a=5.7036*nx, b=5.7036*ny, c=5.7036*nz))

print = lambda *args: (builtins.print(*args) if sim.topology.rank == 0 else None)
print ('nlistdemo.py running with processor grid: ', sim.grid)

pm = LPMD.PluginManager()

natoms = nx*ny*nz*4
gen = pm.Load('fcc', options={ 'nx': str(nx), 'ny': str(ny), 'nz': str(nz), 'a': '5.7036' })
cellman = pm.Load(cmname, options={'cutoff': str(cutoff), 'nx': '15', 'ny': '15', 'nz': '15' })
writer = pm.Load('xyz', options={ 'file': 'test.xyz' })

gen.Generate(sim, modify=True)
print ('Total Size: ', sim.atomset.totalsize)
assert sim.atomset.totalsize == natoms
pm['centeratoms'].Apply(sim, modify=True)

sim.cellmanager = cellman       # Asigna el cellmanager
sim.atomset.boundarylength = bl

sim.cellmanager.UpdateCell(sim, useboundaries=True)
nlist = sim.cellmanager.BuildNeighborsList(sim)
writer.WriteHeader(sim)
writer.WriteCell(sim)
writer.CloseFile(sim)

ulist = [ nlist[z] for z in range(nlist.size) if nlist[z][5] > nlist[z][4] ]
gather_ulist = sim.topology.GatherFromAll(0, ulist)
half_list = list()
for z in gather_ulist: half_list.extend(z)

print ('Number of neighbor pairs: ', len(half_list))
ci = [ 0 for i in range(natoms) ]
for z in half_list:
    ci[z[4]] += 1
    ci[z[5]] += 1

wrong = tuple(i for i in range(natoms) if ci[i] != nc)
if len(wrong) > 0:
   print ('%d atoms have the wrong coordination!' % len(wrong))
   print ('Min %d, Max %d' % (min(wrong), max(wrong)))
   for z in wrong: print (z, ci[z], 'LESS' if ci[z] < nc else 'MORE')

LPMD.FinishCommunication()

