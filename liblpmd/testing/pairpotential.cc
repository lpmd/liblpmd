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

double PairPotential::energy(Configuration & conf) { return energycache; }

void PairPotential::UpdateForces(Configuration & conf)
{
 const double forcefactor = GlobalSession["forcefactor"];
 BasicParticleSet & atoms = conf.Atoms();
 const long int n = atoms.Size();
 energycache = 0.0;
 double stress[3][3];
 for (int i=0;i<3;++i)
 {
  for (int j=0;j<3;++j) stress[i][j]=0.0e0;
 }
 double cutoff=GetCutoff();

#define MAX_THREADS 32
 double etmp[MAX_THREADS]={0.0},tmpvir[MAX_THREADS]={0.0};
 int tid=0,nthreads=1;

#ifdef _OPENMP
#pragma omp parallel private (tid)
#endif
 {
#ifdef _OPENMP
  nthreads = omp_get_num_threads();
  tid = omp_get_thread_num();
#endif
  for (long i = tid; i < n; i = i + nthreads )
  {
   NeighborList nlist;
   conf.GetCellManager().BuildNeighborList(conf, i, nlist, false, cutoff);
   for (long k=0;k<nlist.Size();++k)
   {
    AtomPair nn = nlist[k];
    if (AppliesTo(atoms[i].Z(), nn.j->Z()) && nn.r2 < cutoff*cutoff) 
    {
     etmp[tid] += pairEnergy(sqrt(nn.r2));
     Vector ff = pairForce(nn.rij);
     atoms[i].Acceleration() += ff*(forcefactor/atoms[i].Mass());
     nn.j->Acceleration() -= ff*(forcefactor/nn.j->Mass());
     tmpvir[tid] -= Dot(nn.rij, ff);
     for (int l=0;l<3;++l)
     {
      stress[0][l] += -(nn.rij)[0]*ff[l];
      stress[1][l] += -(nn.rij)[1]*ff[l];
      stress[2][l] += -(nn.rij)[2]*ff[l];
     }
    }
   }
  }
 }
 double & config_virial = conf.Virial();
 for (int i=0 ; i < nthreads ; ++i ) {energycache += etmp[i];config_virial += tmpvir[i];}
 Matrix & config_stress = conf.StressTensor();
 for (int p=0;p<3;p++)
   for (int q=0;q<3;q++) config_stress.Set(q, p, config_stress.Get(q, p)+stress[q][p]);
}

