//
//
//

#include <lpmd/session.h>
#include <lpmd/pairpotential.h>
#include <lpmd/atompair.h>
#include <lpmd/matrix.h>

using namespace lpmd;

PairPotential::PairPotential() { }

PairPotential::~PairPotential() { }

double PairPotential::energy(Configuration & conf) { return energycache; }

void PairPotential::UpdateForces(Configuration & conf)
{
 const double forcefactor = GlobalSession["forcefactor"];
 Vector ff, acci, accj;
 BasicParticleSet & atoms = conf.Atoms();
 const long int n = atoms.Size();
 energycache = 0.0;
 double tmpvir = 0.0;
 double stress[3][3];
 for (int i=0;i<3;i++)
 {
  for (int j=0;j<3;j++) stress[i][j]=0.0e0;
 }
 for (long i=0;i<n;++i)    // was i<n
 {
  NeighborList & nlist = conf.Neighbors(i, false, GetCutoff());
  for (long int k=0;k<nlist.Size();++k)
  {
   const AtomPair & nn = nlist[k];
   if (AppliesTo(atoms[i].Z(), nn.j->Z()) && nn.r < GetCutoff()) 
   {
    energycache += pairEnergy(nn.r);
    ff = pairForce(nn.rij);
    atoms[i].Acceleration() += ff*(forcefactor/atoms[i].Mass());
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
 }
 double & config_virial = conf.Virial();
 config_virial += tmpvir;
 Matrix & config_stress = conf.StressTensor();
 for (int p=0;p<3;p++)
   for (int q=0;q<3;q++) config_stress.Set(q, p, config_stress.Get(q, p)+stress[q][p]);
}

