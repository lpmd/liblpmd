/*
 * Ejemplo simple de dinamica molecular usando el API de liblpmd
 */

#include <lpmd/pluginmanager.h>
#include <lpmd/simulationcell.h>
#include <lpmd/cellgenerator.h>
#include <lpmd/cellmanager.h>

#include <iostream>

#define RCUT 4.15

using namespace lpmd;

int main()
{
 PluginManager pm;

 SimulationCell cell;
 cell.GetCell()[0] = 17.1191*e1;
 cell.GetCell()[1] = 17.1191*e2;
 cell.GetCell()[2] = 17.1191*e3;

 // Carga de plugins con sus parametros
 pm.LoadPlugin("minimumimage", "cutoff 8.5");
 pm.LoadPlugin("crystalfcc", "symbol Ar nx 3 ny 3 nz 3");

 CellManager & cm = CastModule<CellManager>(pm["minimumimage"]);
 cell.SetCellManager(cm);

 CellGenerator & cg = CastModule<CellGenerator>(pm["crystalfcc"]);
 cg.Generate(cell);

 for (long int i=0;i<cell.size();++i)
 {
  std::vector<Neighbor> nlist;
  cell.BuildNeighborList(i, nlist, true, RCUT);
  if (nlist.size() != 12) 
  {
   std::cerr << "DEBUG Atom " << i << ", wrong number of neighbors: " << nlist.size() << ", should be 12\n";
  }
  else 
  {
   for (int k=0;k<nlist.size();++k) assert(fabs(nlist[k].r-4.03501) < 0.0001);
  }
  assert(nlist.size() == 12);
 }

 return 0;
}

