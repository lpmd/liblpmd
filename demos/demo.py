#!/usr/bin/env python3

import LPMD 

cell = LPMD.Cell(a=23.3, b=25.0, c=17.5)
assert cell.IsOrthogonal()
assert len(cell) == 3
print (cell[0])
print (cell[1])
print (cell[2])
print (cell.px, cell.py, cell.pz)
cell.px, cell.pz = False, False
print (cell.px, cell.py, cell.pz)
print (cell.Volume())
cell[1] = (0.0, 37.0, 5.0)
print (cell.Volume())
assert not cell.IsOrthogonal()

#
cell = LPMD.Cell(a=25.0, b=25.0, c=20.0, gamma=120.0)
assert not cell.IsOrthogonal()
print (cell[0])
print (cell[1])
print (cell[2])

