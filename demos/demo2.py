#!/usr/bin/env python3

import sys, LPMD

LPMD.InitializeCommunication(sys.argv)

cell = LPMD.Cell(a=50.0, b=50.0, c=50.0)

grid = (2, 2) # malla de procesadores
top = LPMD.Topology(cell, grid)

if (top.rank == 0):
   print ("Grid is : ", top.RanksAlong(0), "x", top.RanksAlong(1))
   print ("Ranks are: ", top.ranks)

print ("Rank %d: " % top.rank, top.PositionAlong(0), top.PositionAlong(1))

aset = LPMD.AtomSet(managed=True, topology=top)
assert not aset.HasReturnValue()

LPMD.FinishCommunication()

