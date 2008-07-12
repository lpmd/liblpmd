//
//
//

#include <lpmd/pairpotential.h>
#include <lpmd/neighbor.h>
#include <lpmd/simulationcell.h>
#include <lpmd/physunits.h>

using namespace lpmd;

PairPotential::PairPotential() { }

PairPotential::~PairPotential() { }

double PairPotential::energy(SimulationCell & sc) 
{
 double e = 0.0;
 const long n = sc.Size();
 for (long i=0;i<n;++i)
 {
  std::list<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, false, GetCutoff());
  for (std::list<Neighbor>::const_iterator it=nlist.begin();it!=nlist.end();++it)
  {
   const Neighbor & nn = *it;
   if (AppliesTo(sc[i].Species(), nn.j->Species()) && nn.r < GetCutoff())
   {
    double ep = pairEnergy(nn.r);
    e += ep;
   }
  }
 }
 return e;
}

void PairPotential::UpdateForces(SimulationCell & sc)
{
 Vector ff, acci, accj;
 const long n = sc.Size();
 double tmpvir = 0.0;
 double stress[3][3];
 for (int i=0;i<3;i++)
 {
  for (int j=0;j<3;j++) stress[i][j]=0.0e0;
 }
 for (long i=0;i<n;++i)
 {
  std::list<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, false, GetCutoff());
  for (std::list<Neighbor>::const_iterator it=nlist.begin();it!=nlist.end();++it)
  {
   const Neighbor & nn = *it;
   if (AppliesTo(sc[i].Species(), nn.j->Species()) && nn.r < GetCutoff()) 
   {
    ff = pairForce(nn.rij);
    acci = sc[i].Acceleration(); 
    accj = nn.j->Acceleration(); 
    sc.SetAcceleration(i, acci + ff*(FORCEFACTOR/sc[i].Mass()));
    Atom * jpointer = const_cast<Atom *>(nn.j);                  // esto no se debe hacer :)
    jpointer->SetAccel(accj - ff*(FORCEFACTOR/nn.j->Mass()));    // y menos esto :D
    tmpvir -= Dot(nn.rij, ff); // virial de pares
    //if (ff.Mod() > 10.0) throw HorrendousForce(ff.Mod());

    //Asignacion de stress, un for adicional pequeno, 
    //sera mas lento?
    for (int k=0;k<3;k++)
    {
     stress[0][k] += (nn.rij).GetX()*ff.Get(k);
     stress[1][k] += (nn.rij).GetY()*ff.Get(k);
     stress[2][k] += (nn.rij).GetZ()*ff.Get(k);
    }
   }
  }
 }
 sc.AddToVirial(tmpvir);
 for (int i=0;i<3;i++)
 {
  sc.StressTensor(0,i) = stress[0][i];
  sc.StressTensor(1,i) = stress[1][i];
  sc.StressTensor(2,i) = stress[2][i];
 }
}


