//
//
//

#include <lpmd/metalpotential.h>
#include <lpmd/atompair.h>
#include <lpmd/configuration.h>
#include <lpmd/session.h>

using namespace lpmd;

MetalPotential::MetalPotential() { rho=NULL; }

MetalPotential::MetalPotential(const MetalPotential & mp) { rho = NULL; }

MetalPotential::~MetalPotential() {if(rho!=NULL){delete [] rho; rho=NULL;} }

void MetalPotential::Initialize(Configuration & conf)
{
 BasicParticleSet & atoms = conf.Atoms();
 long int n = atoms.Size();
 double rhoi = 0.0e0;
 //Almacena densidad en variable rho de la clase metalpotential.
 delete [] rho;
 rho = new double[n];
 for(long i=0;i<n;i++) rho[i]=0.0e0;
 //Setea las densidades locales de cada atomo.
 for (long i=0;i<n;++i)
 {
  rhoi=0.0e0;
  NeighborList & nlist = conf.Neighbors(i, true, 2*GetCutoff());
  for (long int k=0;k<nlist.Size();++k)
  {
   const AtomPair & nn = nlist[k];
   if (AppliesTo(atoms[i].Z(), nn.j->Z())) rhoi += rhoij(nn.r);
  }
  rho[i]=rhoi;
 }
}

void MetalPotential::VirialEvaluate(Configuration & conf) { }

double MetalPotential::energy(Configuration & conf) { return energycache; }

void MetalPotential::UpdateForces(Configuration & conf)
{
 const double forcefactor = double(GlobalSession["forcefactor"]);
 BasicParticleSet & atoms = conf.Atoms();
 const long n = atoms.Size();
 //Generamos las densidades locales de cada atomo.
 Initialize(conf);
 double tmpvir = 0.0;
 energycache = 0.0;
 double stress[3][3];
 Vector pf,acci,accj,mb;
 long i;

 // FIXME: Construct an "index table" so we don't have to depend on Atom::Index()
 std::map<BasicAtom *, long int> indices;
 for (long int i=0;i<atoms.Size();++i) indices[&atoms[i]] = i;
 //

 for (i=0;i<n;++i)
 {
  NeighborList & nlist = conf.Neighbors(i, false, GetCutoff());
  for (long int k=0;k<nlist.Size();++k)
  {
   const AtomPair & nn = nlist[k];
   if (AppliesTo(atoms[i].Z(), nn.j->Z()))
   {
    energycache += pairEnergy(nn.r);
    pf = PairForce(nn.rij);
    mb = ManyBodies(nn.rij,rho[i],rho[indices[nn.j]]);
    atoms[i].Acceleration() += ((pf+mb)*(forcefactor/atoms[i].Mass()));
    nn.j->Acceleration() -= ((pf+mb)*(forcefactor/nn.j->Mass()));
    tmpvir -= Dot(nn.rij, pf+mb);
    //FIXME : Corregir - Aclarar signo del stress
    for (int q=0;q<3;q++)
    {
     stress[0][q] += -(nn.rij)[0]*(pf+mb)[q];
     stress[1][q] += -(nn.rij)[1]*(pf+mb)[q];
     stress[2][q] += -(nn.rij)[2]*(pf+mb)[q];
    }
   }
  }
 }
 //Calcula de la Energia de Muchos Cuerpos.
 for (i=0;i<n;++i) {energycache += F(rho[i]);}

 // Virial 
 VirialEvaluate(conf);
 double & config_virial = conf.Virial();
 config_virial += tmpvir;
 Matrix & config_stress = conf.StressTensor();
 for (int p=0;p<3;p++)
   for (int q=0;q<3;q++) config_stress.Set(q, p, config_stress.Get(q, p)+stress[q][p]);
}

double MetalPotential::VirialContribution(const double &r, const double & rhoi, const double & rhoj) const { return 0.0; }

double MetalPotential::VirialCorrection(const double & rhobar, const int & N, const double & rhoi) const { return 0.0; }

