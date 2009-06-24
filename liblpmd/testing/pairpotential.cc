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
 Vector ff;
 BasicParticleSet & atoms = conf.Atoms();
 const long int n = atoms.Size();
 energycache = 0.0;
 double tmpvir = 0.0,etmp=0.0e0;
 double stress[3][3];
 for (int i=0;i<3;i++)
 {
  for (int j=0;j<3;j++) stress[i][j]=0.0e0;
 }
 long i,k;
#ifdef _OPENMP
#pragma omp parallel for private ( i,k,ff ) reduction ( + : etmp, tmpvir )
#endif
 for (i=0;i<n;++i)
 {
  NeighborList & nlist = conf.Neighbors(i, false, GetCutoff());
  for (k=0;k<nlist.Size();++k)
  {
   const AtomPair & nn = nlist[k];
   if (AppliesTo(atoms[i].Z(), nn.j->Z()) && nn.r < GetCutoff()) 
   {
    etmp += pairEnergy(nn.r);
    ff = pairForce(nn.rij);
    atoms[i].Acceleration() += ff*(forcefactor/atoms[i].Mass());
    nn.j->Acceleration() -= ff*(forcefactor/nn.j->Mass());
    tmpvir += Dot(nn.rij, ff);
    for (int k=0;k<3;k++)
    {
     stress[0][k] += -(nn.rij)[0]*ff[k];
     stress[1][k] += -(nn.rij)[1]*ff[k];
     stress[2][k] += -(nn.rij)[2]*ff[k];
    }
   }
  }
 }
 energycache += etmp;
 double & config_virial = conf.Virial();
 config_virial += tmpvir;
 Matrix & config_stress = conf.StressTensor();
 for (int p=0;p<3;p++)
   for (int q=0;q<3;q++) config_stress.Set(q, p, config_stress.Get(q, p)+stress[q][p]);
}

