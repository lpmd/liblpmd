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

class Pair
{
 public:
  Pair(BasicAtom * i0, BasicAtom * j0): i(i0), j(j0) { }

  BasicAtom * i;
  BasicAtom * j;
};

void ExtractCorrectPairs(Array<Pair> & ap)
{
 Array<Pair> tmp;
 for (long int i=0;i<ap.Size();++i)
 {
  if ((long int)(ap[i].i) < (long int)(ap[i].j)) tmp.Append(ap[i]); 
 }
 ap = tmp;
}

int main()
{
 PluginManager pm;
 Simulation * simp = GeneralEngine(108, Atom("Ar"));
 Simulation & md = (*simp);

 BasicCell & cell = md.Cell();
 cell[0] = 17.1191*e1;
 cell[1] = 17.1191*e2;
 cell[2] = 17.1191*e3;

 BasicParticleSet & atoms = md.Atoms();

 assert(atoms.Size() == 108);
 CellGenerator & cg = pm.LoadPluginAs<CellGenerator>("crystalfcc", "symbol Ar nx 3 ny 3 nz 3");
 cg.Generate(md);

 md.SetCellManager(pm.LoadPluginAs<CellManager>("minimumimage", "cutoff 8.5"));

 Array<Pair> pairs;
 for (long int i=0;i<atoms.Size();++i)
 {
  NeighborList & nlist = md.Neighbors(i, true, RCUT);
  if (nlist.Size() != 12) 
  {
   std::cerr << "DEBUG Atom " << i << ", wrong number of neighbors: " << nlist.Size() << ", should be 12\n";
  }
  else 
  {
   for (int k=0;k<nlist.Size();++k) 
   {
    assert(fabs(nlist[k].r-4.03501) < 0.0001);
    assert(nlist[k].i != nlist[k].j);
    pairs.Append(Pair(nlist[k].i, nlist[k].j));
   }
  }
  assert(nlist.Size() == 12);
 }

 Array<Pair> pairs2;
 for (long int i=0;i<atoms.Size();++i)
 {
  NeighborList & nlist = md.Neighbors(i, false, RCUT);
  for (int k=0;k<nlist.Size();++k) 
  {
   assert(fabs(nlist[k].r-4.03501) < 0.0001);
   assert(nlist[k].i != nlist[k].j);
   pairs2.Append(Pair(nlist[k].i, nlist[k].j));
  }
 }

 assert(pairs.Size() == 2*pairs2.Size());
 ExtractCorrectPairs(pairs);
 ExtractCorrectPairs(pairs2);

 std::cerr << "DEBUG " << pairs.Size() << " " << pairs2.Size() << '\n';
 assert(pairs.Size() == pairs2.Size());

 std::cerr << "NeighborList passed OK!\n";

 delete simp;
 return 0;
}

