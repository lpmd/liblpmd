//
//
//

#include <lpmd/metalpotential.h>
#include <lpmd/neighbor.h>
#include <lpmd/simulationcell.h>
#include <lpmd/physunits.h>

using namespace lpmd;

MetalPotential::MetalPotential() { rho=NULL; }

MetalPotential::MetalPotential(const MetalPotential & mp) { rho = NULL; }

MetalPotential::~MetalPotential() {if(rho!=NULL){delete [] rho; rho=NULL;} }

void MetalPotential::Initialize(SimulationCell & sc)
{
 int n = sc.Size();
 //Almacena densidad en variable rho de la clase metalpotential.
 delete [] rho;
 rho = new double[n];
 for(int i=0;i<n;i++) rho[i]=0.0e0;
 //Setea las densidades locales de cada atomo.
 for (long i=0;i<n;++i)
 {
  double rhoi=0.0e0;
  std::list<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, true);
  for (std::list<Neighbor>::const_iterator it=nlist.begin();it!=nlist.end();++it)
  {
   const Neighbor &nn = *it;
   if (AppliesTo(sc[i].Species(), nn.j->Species()) && nn.r < GetCutoff())
   {
    rhoi += rhoij(nn.r);
   }
  }
  rho[i]=rhoi + deltarhoi(sc.ParticleDensity());
 }
}

void MetalPotential::VirialEvaluate(SimulationCell & sc)
{
 double vc = 0.0;
 const long n = sc.Size();
 double mrho = 0.0;
 for (long i=0;i<n;++i) mrho+=rho[i];
 mrho/=double(n);
 vc = VirialCorrection(sc.ParticleDensity(),n,mrho);
 sc.AddToVirial(vc);
}

double MetalPotential::energy(SimulationCell & sc) 
{
 double e = 0.0;
 const long n = sc.Size();
 //Calculo de la Energia de Pares U1
 for (long i=0;i<n;++i)
 {
  std::list<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, false);
  for (std::list<Neighbor>::const_iterator it=nlist.begin();it!=nlist.end();++it)
  {
   const Neighbor & nn = *it;
   if (AppliesTo(sc[i].Species(), nn.j->Species()) && nn.r < GetCutoff())
   {
    e += pairEnergy(nn.r);
   }
  }
 }
 //Anade correccion a la energia de pares.
 e+=deltaU1(sc.ParticleDensity(),n);
 //Calcula de la Energia de Muchos Cuerpos.
 for (long i=0;i<n;++i)
 {
  e += F(rho[i]);
 }
 //Evalua el termino del Virial, para asignar correccion a sc.
 VirialEvaluate(sc);
 return e;
}

void MetalPotential::UpdateForces(SimulationCell & sc)
{
 const long n = sc.Size();
 Initialize(sc);
 double tmpvir = 0.0;
 for (long i=0;i<n;++i)
 {
  std::list<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, false);
  //Ahora continuamos el calculo.
  for (std::list<Neighbor>::const_iterator it=nlist.begin();it!=nlist.end();++it)
  {
   const Neighbor & nn = *it;
   if (AppliesTo(sc[i].Species(), nn.j->Species())) 
   {
    Vector pf, acci, accj, mb;
    pf = PairForce(nn.rij);
    mb = ManyBodies(nn.rij,rho[i],rho[nn.j->Index()]);
    acci = sc[i].Acceleration();
    accj = nn.j->Acceleration(); 
    sc.SetAcceleration(i, acci + (pf+mb)*(FORCEFACTOR/sc[i].Mass()));
    Atom * jpointer = const_cast<Atom *>(nn.j);                         // FIXME esto no se debe hacer :)
    jpointer->SetAccel(accj - (pf+mb)*(FORCEFACTOR/nn.j->Mass()));    // FIXME y menos esto :D
    tmpvir -= Dot(nn.rij, pf+mb);  // virial de pares
   }
  }
 }
 sc.AddToVirial(tmpvir);
}

double MetalPotential::VirialContribution(const double &r, const double & rhoi, const double & rhoj) const { return 0.0; }

double MetalPotential::VirialCorrection(const double & rhobar, const int & N, const double & rhoi) const { return 0.0; }

