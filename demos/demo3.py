#!/usr/bin/env python3

import LPMD

nlist = LPMD.NeighborList(5)

nlist[0] = ( (1.0, 2.0, 3.0), 42.0, 10, 20, False)
print (nlist[0])

