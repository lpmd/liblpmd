//
//
//

#include <lpmd/session.h>
#include <lpmd/pairpotential.h>
#include <lpmd/atompair.h>
#include <lpmd/matrix.h>
#ifdef _OPENMP
#include <omp.h>
#endif

using namespace lpmd;

PairPotential::PairPotential() { }

PairPotential::~PairPotential() { }

double PairPotential::energy(Configuration & conf) { assert(&conf != 0); return energycache; }//icc 869

double PairPotential::AtomEnergy(Configuration & conf, long i)
{
 double etmp = 0.0;
 NeighborList nlist;
 const double cutoff = GetCutoff();
 BasicParticleSet & atoms = conf.Atoms();
 conf.GetCellManager().BuildNeighborList(conf, i, nlist, true, cutoff);
 for (long k=0;k<nlist.Size();++k)
 {
  AtomPair nn = nlist[k];
  if (AppliesTo(atoms[i].Z(), nn.j->Z()) && nn.r2 < cutoff*cutoff) 
     etmp += pairEnergy(sqrt(nn.r2));
 }
 return etmp;
}

void PairPotential::UpdateForces(Configuration & conf)
{
 const double forcefactor = GlobalSession["forcefactor"];
 BasicParticleSet & atoms = conf.Atoms();
 const long int n = atoms.Size();
 energycache = 0.0;
 double stress[3][3];
 double etmp = 0.0e0,tmpvir = 0.0e0;
 for (int i=0;i<3;++i)
 {
  for (int j=0;j<3;++j) stress[i][j]=0.0e0;
 }
 const double cutoff = GetCutoff();

#ifdef _OPENMP
#pragma omp parallel for reduction ( + : etmp, tmpvir )
#endif
 for (long i = 0; i < n; ++i )
 {
  NeighborList nlist;
  conf.GetCellManager().BuildNeighborList(conf, i, nlist, false, cutoff);
  for (long k=0;k<nlist.Size();++k)
  {
   AtomPair nn = nlist[k];
   if (AppliesTo(atoms[i].Z(), nn.j->Z()) && nn.r2 < cutoff*cutoff) 
   {
    etmp += pairEnergy(sqrt(nn.r2));
    Vector ff = pairForce(nn.rij);
    atoms[i].Acceleration() += ff*(forcefactor/atoms[i].Mass());
    nn.j->Acceleration() -= ff*(forcefactor/nn.j->Mass());
    tmpvir -= Dot(nn.rij, ff);
    for (int l=0;l<3;++l)
    {
     stress[0][l] += -(nn.rij)[0]*ff[l];
     stress[1][l] += -(nn.rij)[1]*ff[l];
     stress[2][l] += -(nn.rij)[2]*ff[l];
    }
   }
  }
 }
 double & config_virial = conf.Virial();
 energycache += etmp;
 config_virial += tmpvir;
 Matrix & config_stress = conf.StressTensor();
 for (int p=0;p<3;p++)
   for (int q=0;q<3;q++) config_stress.Set(q, p, config_stress.Get(q, p)+stress[q][p]);
}

