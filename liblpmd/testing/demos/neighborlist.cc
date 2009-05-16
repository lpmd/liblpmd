/*
 * Ejemplo simple de dinamica molecular usando el API de liblpmd
 */

#include <lpmd/simulation.h>
#include <lpmd/cellgenerator.h>
#include <lpmd/cellmanager.h>
#include <lpmd/pluginmanager.h>

#include <iostream>

#define RCUT 4.15

using namespace lpmd;

int main()
{
 PluginManager pm;
 Simulation * simp = GeneralEngine(108, Atom("Ar"));
 Simulation & md = (*simp);

 BasicCell & cell = md.Cell();
 cell[0] = 17.1191*e1;
 cell[1] = 17.1191*e2;
 cell[2] = 17.1191*e3;

 md.SetCellManager(pm.LoadPluginAs<CellManager>("minimumimage", "cutoff 8.5"));

 BasicParticleSet & atoms = md.Atoms();

 assert(atoms.Size() == 108);
 CellGenerator & cg = pm.LoadPluginAs<CellGenerator>("crystalfcc", "symbol Ar nx 3 ny 3 nz 3");
 cg.Generate(md);

 for (long int i=0;i<atoms.Size();++i)
 {
  NeighborList & nlist = md.Neighbors(i, true, RCUT);
  if (nlist.Size() != 12) 
  {
   std::cerr << "DEBUG Atom " << i << ", wrong number of neighbors: " << nlist.Size() << ", should be 12\n";
  }
  else 
  {
   for (int k=0;k<nlist.Size();++k) assert(fabs(nlist[k].r-4.03501) < 0.0001);
  }
  assert(nlist.Size() == 12);
 }
 std::cerr << "NeighborList passed OK!\n";

 delete simp;
 return 0;
}

