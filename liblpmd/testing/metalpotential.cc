//
//
//

#include <lpmd/metalpotential.h>
#include <lpmd/neighbor.h>
#include <lpmd/simulationcell.h>
#include <lpmd/session.h>

using namespace lpmd;

MetalPotential::MetalPotential() { rho=NULL; }

MetalPotential::MetalPotential(const MetalPotential & mp) { rho = NULL; }

MetalPotential::~MetalPotential() { if (rho!=NULL) { delete [] rho; rho=NULL; } }

void MetalPotential::Initialize(SimulationCell & sc)
{
 Potential::Initialize(sc);
 const long int n = sc.size();
 //Almacena densidad en variable rho de la clase metalpotential.
 delete [] rho;
 rho = new double[n];
 for(long int i=0;i<n;i++) rho[i]=0.0e0;
 //Setea las densidades locales de cada atomo.
 double partdens = sc.ParticleDensity();
 double drhoi = deltarhoi(partdens);
 for (long int i=0;i<n;++i)
 {
  const Atom & at = sc[i];
  double rhoi=0.0e0;
  std::vector<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, true, GetCutoff());
  for (unsigned long int k=0;k<nlist.size();++k)
  {
   const Neighbor &nn = nlist[k];
   if (AppliesTo(at.Species(), nn.j->Species()) && nn.r < GetCutoff()) rhoi += rhoij(nn.r);
  }
  rho[i] = rhoi + drhoi;
 }
}

void MetalPotential::VirialEvaluate(SimulationCell & sc)
{
 double vc = 0.0;
 const long int n = sc.size();
 double mrho = 0.0;
 for (long i=0;i<n;++i) mrho+=rho[i];
 mrho/=double(n);
 vc = VirialCorrection(sc.ParticleDensity(),n,mrho);
 sc.AddToVirial(vc);
}

double MetalPotential::energy(SimulationCell & sc) { return energycache; }

void MetalPotential::UpdateForces(SimulationCell & sc)
{
 const double forcefactor = GlobalSession.GetDouble("forcefactor");
 Initialize(sc);
 double tmpvir = 0.0;
 energycache = 0.0;
 const long int n = sc.size();
 double stress[3][3];
 for (long i=0;i<n;++i)
 {
  const Atom & at = sc[i];
  std::vector<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, false, GetCutoff());
  //Ahora continuamos el calculo.
  for (unsigned long int k=0;k<nlist.size();++k)
  {
   const Neighbor & nn = nlist[k];
   if (AppliesTo(at.Species(), nn.j->Species())) 
   {
    Vector pf, acci, accj, mb;
    pf = PairForce(nn.rij);
    mb = ManyBodies(nn.rij,rho[i],rho[nn.j->Index()]);
    acci = at.Acceleration();
    accj = nn.j->Acceleration(); 
    sc.SetAcceleration(i, acci + (pf+mb)*(forcefactor/at.Mass()));
    Atom * jpointer = const_cast<Atom *>(nn.j);                         // FIXME esto no se debe hacer :)
    jpointer->SetAccel(accj - (pf+mb)*(forcefactor/nn.j->Mass()));    // FIXME y menos esto :D
    energycache += pairEnergy(nn.r); // energia
    tmpvir -= Dot(nn.rij, pf+mb);  // virial de pares
    //Asignacion de stress, un for adicional pequeno, 
    //sera mas lento?
    for (int k=0;k<3;k++)
    {
     stress[0][k] += (nn.rij).GetX()*(pf+mb).Get(k);
     stress[1][k] += (nn.rij).GetY()*(pf+mb).Get(k);
     stress[2][k] += (nn.rij).GetZ()*(pf+mb).Get(k);
    }
   }
  }
 }
 //Anade correccion a la energia de pares.
 energycache += deltaU1(sc.ParticleDensity(), n);
 // calcula la Energia de Muchos Cuerpos.
 for (long i=0;i<n;++i) energycache += F(rho[i]);
 // Asigna virial
 sc.AddToVirial(tmpvir);
 // Evalua el termino del virial, para asignar correccion a sc.
 VirialEvaluate(sc);
 for (int i=0;i<3;i++)
 {
  sc.StressTensor(0,i) = stress[0][i];
  sc.StressTensor(1,i) = stress[1][i];
  sc.StressTensor(2,i) = stress[2][i];
 }
}

double MetalPotential::VirialContribution(const double &r, const double & rhoi, const double & rhoj) const { return 0.0; }

double MetalPotential::VirialCorrection(const double & rhobar, const int & N, const double & rhoi) const { return 0.0; }

