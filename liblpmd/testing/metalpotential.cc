//

//
//

#include <lpmd/metalpotential.h>
#include <lpmd/atompair.h>
#include <lpmd/configuration.h>
#include <lpmd/session.h>

using namespace lpmd;

MetalPotential::MetalPotential() { rho=NULL;invrho=NULL; }

MetalPotential::MetalPotential(const MetalPotential & mp) { assert(&mp != 0); rho = NULL; invrho = NULL; }//icc 869

MetalPotential::~MetalPotential() {if(rho!=NULL){delete [] rho; rho=NULL;}; if(invrho!=NULL){delete [] invrho; invrho=NULL;} }

void MetalPotential::Initialize(Configuration & conf)
{
 assert(&conf != 0); //icc 869
}

void MetalPotential::VirialEvaluate(Configuration & conf) { assert(&conf != 0); }//icc 869

double MetalPotential::energy(Configuration & conf) { assert(&conf != 0); return energycache; }//icc 869

double MetalPotential::AtomEnergy(Configuration & conf, long i)
{
 double etmp = 0.0, rhoi = 0.0;
 NeighborList nlist;
 BasicParticleSet & atoms = conf.Atoms();
 conf.GetCellManager().BuildNeighborList(conf, i, nlist, true, GetCutoff());
 for (long k=0;k<nlist.Size();++k) rhoi += rhoij(sqrt(nlist[k].r2));
 for (long k=0;k<nlist.Size();++k)
 {
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

 //Almacena densidad en variable rho y el inverso en invrho.
 delete [] rho;
 delete [] invrho;
 rho = new double[n];
 invrho = new double[n];
 for(long i=0;i<n;++i) {rho[i]=0.0e0; invrho[i]=0.0e0; indices[&atoms[i]]=i;}

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
  rho[i] = rhoi;
  invrho[i] = 1/rho[i];
 }

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
    double ir = 1/r;
    Vector norm = nn.rij;
    norm.Normalize();
    etmp += pairEnergy(r);
    Vector pf = PairForce(norm, ir);
    Vector mb = ManyBodies(norm, invrho[i], invrho[indices[nn.j]], ir);
    atoms[i].Acceleration() += ((pf+mb)*(forcefactor/atoms[i].Mass()));
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
 energycache += etmp + etmp2;
 // Virial 
 VirialEvaluate(conf);
 double & config_virial = conf.Virial();
 config_virial += tmpvir;
 Matrix & config_stress = conf.StressTensor();
 for (int p=0;p<3;p++)
   for (int q=0;q<3;q++) config_stress.Set(q, p, config_stress.Get(q, p)+stress[q][p]);
}

double MetalPotential::VirialContribution(const double &r, const double & rhoi, const double & rhoj) const 
{ 
 assert(&r != 0); //icc 869
 assert(&rhoi != 0); //icc 869
 assert(&rhoj != 0); //icc 869
 return 0.0;
}

double MetalPotential::VirialCorrection(const double & rhobar, const int & N, const double & rhoi) const 
{
 assert(&rhobar != 0); //icc 869
 assert(&N != 0); //icc 869
 assert(&rhoi != 0);  //icc 869
 return 0.0; 
}

