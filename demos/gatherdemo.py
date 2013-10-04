#!/usr/bin/env python3

import sys, builtins, LPMD

nprocs = LPMD.InitializeCommunication(sys.argv)

top = LPMD.Topology(cell=LPMD.Cell(a=1.0, b=1.0, c=1.0), grid=LPMD.AutoGrid(nprocs))

class MiContenedor:
    def __init__(self): self.firma = 42

object = MiContenedor()
object.numero = 42.23478
object.procer = top.rank
object.paridad = ('par' if top.rank % 2 == 0 else 'impar')
object.tupla = ( 1, 0, 1 )

gathered = top.GatherFromAll(0, object)

# Todos los objetos fueron reunidos en el rank 0 en una tupla
# En los otros ranks se devolvio una tupla vacia
for i, z in enumerate(gathered): 
    print ('I am rank %d, got from rank %d: ' % (top.rank, i), z)
    print ('... firma = ', z.firma)
    print ('... numero = ', z.numero)
    print ('... procer = ', z.procer)
    print ('... paridad = ', z.paridad)
    print ('... tupla = ', z.tupla)

LPMD.FinishCommunication()

