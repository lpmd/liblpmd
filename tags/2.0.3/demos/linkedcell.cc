/*
 * Ejemplo simple de dinamica molecular usando el API de liblpmd
 */

#include <lpmd/simulationbuilder.h>
#include <lpmd/cellgenerator.h>
#include <lpmd/cellmanager.h>
#include <lpmd/pluginmanager.h>
#include <lpmd/manipulations.h>

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

template <typename AtomContainer, typename CellType> void GenerateFCC(AtomContainer & atoms, CellType & cell)
{
 long int cc = 0;
 double ax = 1.0/3.0;
 double ay = 1.0/3.0;
 double az = 1.0/3.0;
 for (long k=0;k<3;++k)
  for (long j=0;j<3;++j)
   for (long i=0;i<3;++i)
   {
    atoms[cc++].Position() = cell.ScaleByCell(Vector((double(i)+0.5)*ax, (double(j)+0.5)*ay, (double(k)+0.5)*az));
    atoms[cc++].Position() = cell.ScaleByCell(Vector((double(i)+0.5)*ax, double(j)*ay, (double(k)+1.0)*az));
    atoms[cc++].Position() = cell.ScaleByCell(Vector((double(i)+1.0)*ax, double(j)*ay, (double(k)+0.5)*az));
    atoms[cc++].Position() = cell.ScaleByCell(Vector((double(i)+1.0)*ax, (double(j)+0.5)*ay, (double(k)+1.0)*az));
   }
 CenterByCenterOfMass(atoms, cell);
}

template <typename AtomContainer, typename CellType> void CheckAllInside(AtomContainer & atoms, CellType & cell)
{
 for (int q=0;q<atoms.Size();++q)
 {
  Vector vpos = cell.Fractional(atoms[q].Position());
  assert((vpos[0] >= 0.0) && (vpos[0] <= 1.0));
  assert((vpos[1] >= 0.0) && (vpos[1] <= 1.0));
  assert((vpos[2] >= 0.0) && (vpos[2] <= 1.0));
 }
}

int main()
{
 PluginManager pm;
 Simulation & md = SimulationBuilder::CreateFixedOrthogonal(108, Atom("Ar"));

 BasicCell & cell = md.Cell();
 cell[0] = 17.1191*e1;
 cell[1] = 17.1191*e2;
 cell[2] = 17.1191*e3;

 BasicParticleSet & atoms = md.Atoms();

 assert(atoms.Size() == 108);
 GenerateFCC(atoms, cell);
 CheckAllInside(atoms, cell);

 //md.SetCellManager(pm.LoadPluginAs<CellManager>("minimumimage", "cutoff 8.5"));
 md.SetCellManager(pm.LoadPluginAs<CellManager>("linkedcell", "cutoff 8.5 nx 1 ny 1 nz 1"));

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
    assert(fabs(sqrt(nlist[k].r2)-4.03501) < 0.0001);
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
   assert(fabs(sqrt(nlist[k].r2)-4.03501) < 0.0001);
   assert(nlist[k].i != nlist[k].j);
   pairs2.Append(Pair(nlist[k].i, nlist[k].j));
  }
 }

 assert(pairs.Size() == 2*pairs2.Size());
// ExtractCorrectPairs(pairs);
// ExtractCorrectPairs(pairs2);

// std::cerr << "DEBUG " << pairs.Size() << " " << pairs2.Size() << '\n';
// assert(pairs.Size() == pairs2.Size());

 std::cerr << "LinkedCell Plugin passed OK!\n";

 return 0;
}

