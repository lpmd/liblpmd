//
//
//

#include <lpmd/metalpotential.h>
#include <lpmd/simulation.h>
#include <lpmd/atompair.h>
#include <lpmd/session.h>

using namespace lpmd;

MetalPotential::MetalPotential() { rho=NULL; }

MetalPotential::MetalPotential(const MetalPotential & mp) { rho = NULL; }

MetalPotential::~MetalPotential() { if (rho!=NULL) { delete [] rho; rho=NULL; } }

void MetalPotential::Initialize(Configuration & conf)
{
 Potential::Initialize(conf);
 BasicParticleSet & atoms = conf.Atoms();
 BasicCell & cell = conf.Cell();
 const long int n = atoms.Size();
 //Almacena densidad en variable rho de la clase metalpotential.
 delete [] rho;
 rho = new double[n];
 for(long int i=0;i<n;i++) rho[i]=0.0e0;
 //Setea las densidades locales de cada atomo.
 double partdens = atoms.Size()/cell.Volume(); // was sc.ParticleDensity();
 double drhoi = deltarhoi(partdens);
 for (long int i=0;i<n;++i)
 {
  const Atom & at = atoms[i];
  double rhoi=0.0e0;
  NeighborList & nlist = conf.Neighbors(i, true, GetCutoff());
  for (long int k=0;k<nlist.Size();++k)
  {
   const AtomPair & nn = nlist[k];
   if (AppliesTo(at.Z(), nn.j->Z()) && nn.r < GetCutoff()) rhoi += rhoij(nn.r);
  }
  rho[i] = rhoi + drhoi;
 }
}

void MetalPotential::VirialEvaluate(Configuration & conf)
{
// double vc = 0.0;
 BasicParticleSet & atoms = conf.Atoms();
 const long int n = atoms.Size();
 double mrho = 0.0;
 for (long i=0;i<n;++i) mrho+=rho[i];
 mrho/=double(n);
#warning "COMENTADO VIRIAL EN METALPOTENTIAL"
// vc = VirialCorrection(sc.ParticleDensity(),n,mrho);
// sc.AddToVirial(vc);
}

double MetalPotential::energy(Configuration & conf) { return energycache; }

void MetalPotential::UpdateForces(Configuration & conf)
{
 const double forcefactor = GlobalSession.GetDouble("forcefactor");
 Initialize(conf);
 BasicParticleSet & atoms = conf.Atoms();
 BasicCell & cell = conf.Cell();
 double tmpvir = 0.0;
 energycache = 0.0;
 const long int n = atoms.Size();
 double stress[3][3];
 for (long i=0;i<n;++i)
 {
  const Atom & at = atoms[i];
  NeighborList & nlist = conf.Neighbors(i, false, GetCutoff());
  //Ahora continuamos el calculo.
  for (long int k=0;k<nlist.Size();++k)
  {
   const AtomPair & nn = nlist[k];
   if (AppliesTo(at.Z(), nn.j->Z())) 
   {
    Vector pf, acci, accj, mb;
    pf = PairForce(nn.rij);
    mb = ManyBodies(nn.rij, rho[i], rho[nn.j_index]);
    acci = at.Acceleration();
    accj = nn.j->Acceleration(); 
    atoms[i].Acceleration() = acci + (pf+mb)*(forcefactor/at.Mass());
    nn.j->Acceleration() = accj - (pf+mb)*(forcefactor/nn.j->Mass());
    energycache += pairEnergy(nn.r); // energia
    tmpvir -= Dot(nn.rij, pf+mb);  // virial de pares
    //Asignacion de stress, un for adicional pequeno, 
    //sera mas lento?
    for (int k=0;k<3;k++)
    {
     stress[0][k] += (nn.rij)[0]*(pf+mb)[k];
     stress[1][k] += (nn.rij)[1]*(pf+mb)[k];
     stress[2][k] += (nn.rij)[2]*(pf+mb)[k];
    }
   }
  }
 }
 //Anade correccion a la energia de pares.
 energycache += deltaU1(atoms.Size()/cell.Volume(), n);
 // calcula la Energia de Muchos Cuerpos.
 for (long i=0;i<n;++i) energycache += F(rho[i]);
 // Asigna virial
 // sc.AddToVirial(tmpvir);
 // Evalua el termino del virial, para asignar correccion a sc.
#warning "COMENTADO VIRIAL EN METALPOTENTIAL"
 /*
 VirialEvaluate(sc);
 for (int i=0;i<3;i++)
 {
  sc.StressTensor(0,i) = stress[0][i];
  sc.StressTensor(1,i) = stress[1][i];
  sc.StressTensor(2,i) = stress[2][i];
 }
 */
}

double MetalPotential::VirialContribution(const double &r, const double & rhoi, const double & rhoj) const { return 0.0; }

double MetalPotential::VirialCorrection(const double & rhobar, const int & N, const double & rhoi) const { return 0.0; }

