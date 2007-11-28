//
//
//

#include <lpmd/pairpotential.h>
#include <lpmd/physunits.h>

using namespace lpmd;

PairPotential::PairPotential() { }

PairPotential::~PairPotential() { }

double PairPotential::energy(SimulationCell & sc) 
{
 double e = 0.0;
 long n = sc.Size();
 for (long i=0;i<n-1;++i)
   for (long j=i+1;j<n;++j) 
   {
    if (AppliesTo(sc.GetAtom(i).Species(), sc.GetAtom(j).Species())) e += pairEnergy(sc.Distance(i, j));
   }
 return e;
}

void PairPotential::UpdateForces(SimulationCell & sc)
{
 Vector ff, acci, accj;
 long n = sc.Size();
 for (long i=0;i<n-1;++i)
 {
  const Atom & atom_i = sc.GetAtom(i);
  double mi = atom_i.Mass();
  for (long j=i+1;j<n;++j)
  {
   const Atom & atom_j = sc.GetAtom(j);
   double mj = atom_j.Mass();
   if (AppliesTo(atom_i.Species(), atom_j.Species())) 
   {
    ff = pairForce(sc.VectorDistance(i, j));
    acci = atom_i.Acceleration(); 
    accj = atom_j.Acceleration(); 
    sc.SetAcceleration(i, acci + ff*(FORCEFACTOR/mi));
    sc.SetAcceleration(j, accj - ff*(FORCEFACTOR/mj));
   }
  }
 }
}


