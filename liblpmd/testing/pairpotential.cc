//
//
//

#include <lpmd/session.h>
#include <lpmd/pairpotential.h>
#include <lpmd/neighbor.h>

using namespace lpmd;

PairPotential::PairPotential() { }

PairPotential::~PairPotential() { }

double PairPotential::energy(BasicParticleSet & atoms, BasicCell & cell) { return energycache; } 

void PairPotential::UpdateForces(BasicParticleSet & atoms, BasicCell & cell)
{
 const double forcefactor = GlobalSession.GetDouble("forcefactor");
 Vector ff, acci, accj;
 const long int n = atoms.Size();
 energycache = 0.0;
 double tmpvir = 0.0;
 double stress[3][3];
 for (int i=0;i<3;i++)
 {
  for (int j=0;j<3;j++) stress[i][j]=0.0e0;
 }
 for (long i=0;i<n-1;++i)    // was i<n
 {
  /*
  std::vector<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, false, GetCutoff());
  for (unsigned long int k=0;k<nlist.size();++k)
  {
   const Neighbor & nn = nlist[k];
   if (AppliesTo(sc[i].Z(), nn.j->Z()) && nn.r < GetCutoff()) 
   {
    energycache += pairEnergy(nn.r);
    ff = pairForce(nn.rij);
    sc[i].Acceleration() += ff*(forcefactor/sc[i].Mass());
    nn.j->Acceleration() -= ff*(forcefactor/nn.j->Mass());
    tmpvir -= Dot(nn.rij, ff); // virial de pares
    //if (ff.Module() > 10.0) throw HorrendousForce(ff.Module());

    //Asignacion de stress, un for adicional pequeno, 
    //sera mas lento? - El signo parec provenir de la fuerza, ojo con eso
    for (int k=0;k<3;k++)
    {
     stress[0][k] += -(nn.rij)[0]*ff[k];
     stress[1][k] += -(nn.rij)[1]*ff[k];
     stress[2][k] += -(nn.rij)[2]*ff[k];
    }
   }
  }
  */
  // 
  //
  for (long j=i+1;j<n;++j)
  {
   const Vector & v_i = atoms[i].Position();
   const Vector & v_j = atoms[j].Position();
   Vector dist = cell.Displacement(v_i, v_j);
   if (dist.Module() < 8.5)
   {
    energycache += pairEnergy(dist.Module());
    ff = pairForce(dist);
    atoms[i].Acceleration() += ff*(forcefactor/atoms[i].Mass());
    atoms[j].Acceleration() -= ff*(forcefactor/atoms[j].Mass());
   }
  }
 //
 //
 }
 #warning "comentado AddToVirial y StressTensor"
 // sc.AddToVirial(tmpvir);
 /*
 for (int i=0;i<3;i++)
 {
  sc.StressTensor(0,i) = stress[0][i];
  sc.StressTensor(1,i) = stress[1][i];
  sc.StressTensor(2,i) = stress[2][i];
 }
 */
}


