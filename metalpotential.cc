//

//
//

#include <lpmd/metalpotential.h>
#include <lpmd/properties.h>
#include <lpmd/atompair.h>
#include <lpmd/configuration.h>
#include <lpmd/session.h>

using namespace lpmd;

MetalPotential::MetalPotential() { rho=NULL; initial=true; du=0.0e0; drho=0.0e0; dvir=0.0e0; }

MetalPotential::MetalPotential(const MetalPotential & mp) { assert(&mp != 0); rho = NULL; initial=true; du=0.0e0; drho=0.0e0; dvir=0.0e0; }//icc 869

MetalPotential::~MetalPotential() {if(rho!=NULL){delete [] rho; rho=NULL;};}

void MetalPotential::Initialize(Configuration & conf)
{
 assert(&conf != 0); //icc 869
}

double MetalPotential::energy(Configuration & conf) { assert(&conf != 0); return energycache; }//icc 869

double MetalPotential::AtomEnergy(Configuration & conf, long i)
{
 double etmp = 0.0, rhoi = 0.0;
 NeighborList nlist;
 BasicParticleSet & atoms = conf.Atoms();
 conf.GetCellManager().BuildNeighborList(conf, i, nlist, true, GetCutoff());
 for (long k=0;k<nlist.Size();++k)
 {
  rhoi += rhoij(sqrt(nlist[k].r2));
  const AtomPair & nn = nlist[k];
  if (AppliesTo(atoms[i].Z(), nn.j->Z())) etmp += pairEnergy(sqrt(nn.r2));
 }
 etmp += F(rhoi);
 return etmp;
}

void MetalPotential::UpdateForces(Configuration & conf)
{
 const double forcefactor = double(GlobalSession["forcefactor"]);
 BasicParticleSet & atoms = conf.Atoms();
 const long n = atoms.Size();

 energycache = 0.0;
 double stress[3][3];

 // FIXME: Construct an "index table" so we don't have to depend on Atom::Index()
 std::map<BasicAtom *, long int> indices;

 double tmpvir=0.0e0, etmp=0.0e0, etmp2=0.0e0;
 double mpd=0.0; //mean particle density

 delete [] rho;
 rho = new double[n];
 for(long i=0;i<n;++i) {rho[i]=0.0e0; indices[&atoms[i]]=i;}

 //Load Corrections in the begining of the simulation.
 //Correction apply only for suttonchen and gupta poential.
 //and for highly homogeneous systems of this potentials.
 if(initial==true)
 {
  double sinv = 0.0e0;
  //mpd = atoms.Size()/conf.Cell().Volume();
  for(long i=0;i<n;++i)
  {
   double rhoi = 0.0e0;
   NeighborList nlist;
   conf.GetCellManager().BuildNeighborList(conf, i, nlist, true, GetCutoff());
   for (long k=0;k<nlist.Size();++k) 
   {
    rhoi += rhoij(sqrt(nlist[k].r2));
   }
   sinv += 1.0e0/(sqrt(rhoi));
  }
  Vector tmp=UpdateCorrections(mpd,n,sinv);
  //du=tmp[1];
  //dvir=tmp[2];
  GlobalSession.DebugStream() << '\n';
  GlobalSession.DebugStream() << "================================================================================" << '\n';
  GlobalSession.DebugStream() << "=====================  METALLIC  CORRECTIONS.  ================================="<<'\n';
  GlobalSession.DebugStream() << "================================================================================" <<'\n';

  GlobalSession.DebugStream() << "Mean particle density = " << mpd << '\n';
  GlobalSession.DebugStream() << "Sum 1/sqrt(rhoi)      = " << sinv << '\n';
  GlobalSession.DebugStream() << "Delta rhoi            = " << tmp[0] << '\n';
  GlobalSession.DebugStream() << "Delta U1              = " << tmp[1] << '\n';
  GlobalSession.DebugStream() << "Delta Virial 1 + 2    = " << tmp[2] << '\n';
  initial=false;
 }

#ifdef _OPENMP
#pragma omp parallel for
#endif
 for (long i=0;i<n;++i)
 {
  double rhoi = 0.0e0;
  NeighborList nlist;
  conf.GetCellManager().BuildNeighborList(conf, i, nlist, true, GetCutoff());
  for (long k=0;k<nlist.Size();++k)
  {
   rhoi += rhoij(sqrt(nlist[k].r2));
  }
#ifdef _OPENMP
#pragma omp critical
#endif
  rho[i] = rhoi + mpd;
 }
#ifdef _OPENMP
#pragma omp barrier
#endif

#ifdef _OPENMP
#pragma omp parallel for reduction ( + : etmp, tmpvir, etmp2 )
#endif
 for (long i=0;i<n;++i)
 {
  NeighborList nlist;
  conf.GetCellManager().BuildNeighborList(conf, i, nlist, false, GetCutoff());
  for (long k=0;k<nlist.Size();++k)
  {
   AtomPair nn = nlist[k];
   if (AppliesTo(atoms[i].Z(), nn.j->Z()))
   {
    double r = sqrt(nn.r2);
    Vector norm = nn.rij;
    norm.Normalize();
    etmp += pairEnergy(r); 
    Vector pf = PairForce(norm, r);
    Vector mb = ManyBodies(norm, rho[i], rho[indices[nn.j]], r);
#ifdef _OPENMP
#pragma omp critical
#endif
    atoms[i].Acceleration() += ((pf+mb)*(forcefactor/atoms[i].Mass()));
#ifdef _OPENMP
#pragma omp critical
#endif
    nn.j->Acceleration() -= ((pf+mb)*(forcefactor/nn.j->Mass()));
    tmpvir -= Dot(nn.rij, pf+mb);
    for (int q=0;q<3;q++)
    {
     stress[0][q] += -(nn.rij)[0]*(pf+mb)[q];
     stress[1][q] += -(nn.rij)[1]*(pf+mb)[q];
     stress[2][q] += -(nn.rij)[2]*(pf+mb)[q];
    }
   }
  }
  etmp2 += F(rho[i]);
 }
#ifdef _OPENMP
#pragma omp barrier
#endif
 energycache += etmp + du + etmp2;//added energy U1 correction.
 double & config_virial = conf.Virial();
 config_virial += tmpvir + dvir; //added virial (V1+V2) correction. 
 Matrix & config_stress = conf.StressTensor();
 for (int p=0;p<3;p++)
   for (int q=0;q<3;q++) config_stress.Set(q, p, config_stress.Get(q, p)+stress[q][p]);
}
