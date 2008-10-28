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
 long i = 0;
 double rhoi = 0.0e0;
 //Almacena densidad en variable rho de la clase metalpotential.
 delete [] rho;
 rho = new double[n];
 for(i=0;i<n;i++) rho[i]=0.0e0;
 //Setea las densidades locales de cada atomo.
 //Creo que esta bien openMP porque no se reduce nada y cada
 //procesador se encarga de la densidad de "un" atomo a la vez.
#ifdef _OPENMP
#pragma omp parallel for private ( i,rhoi )
#endif
 for (i=0;i<n;++i)
 {
  rhoi=0.0e0;
  std::list<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, true, 2*GetCutoff());
  for (std::list<Neighbor>::const_iterator it=nlist.begin();it!=nlist.end();++it) 
  {
   const Neighbor &nn = *it;
   if (AppliesTo(sc[i].Species(), nn.j->Species()))
   {
    rhoi += rhoij(nn.r);
   }
  }
  rho[i]=rhoi;
 }
}

void MetalPotential::VirialEvaluate(SimulationCell & sc)
{
 //double vc = 0.0;
 //const long n = sc.Size();
 //double mrho = 0.0;
 //for (long i=0;i<n;++i) mrho+=
 //FIXME : El signo no esta explicado!!
 //vc = VirialCorrection(sc.ParticleDensity(),n,mrho);
 //std::cerr << "Virial correction = " << vc << '\n';
 //sc.AddToVirial(vc);
}

double MetalPotential::energy(SimulationCell & sc) 
{
 // Si la energia potencial esta ingresada como metadato en sc, el valor es 
 // todavia valido, simplemente retorna ese valor
 if (sc.MetaData().Defined("pe")) return sc.MetaData().GetDouble("pe");
 ShowWarning("MetalPotential", "Recalculating potential energy... there is a waste of time somewhere..."); 
/*
 double e = 0.0;
 const long n = sc.Size();
 //Calculo de la Energia de Pares U1
 for (long i=0;i<n;++i)
 {
  std::list<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, false, GetCutoff());
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
 //std::cerr << "Llamando a VirialEvaluate" << '\n';
 VirialEvaluate(sc);
 //std::cerr << "Listo!"<< '\n';
 return e;
*/
}

void MetalPotential::UpdateForces(SimulationCell & sc)
{
 const long n = sc.Size();
 //Generamos las densidades locales de cada atomo.
 Initialize(sc);
 double tmpvir = 0.0, ep = 0.0;
 double stress[3][3];
 Vector pf,acci,accj,mb;
 long i;

#ifdef _OPENMP
#pragma omp parallel for private ( i,pf,mb,acci,accj ) reduction ( + : ep,tmpvir )
#endif
 for (i=0;i<n;++i)
 {
  std::list<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, false, GetCutoff());
  for (std::list<Neighbor>::const_iterator it=nlist.begin();it!=nlist.end();++it)
  {
   const Neighbor & nn = *it;
   if (AppliesTo(sc[i].Species(), nn.j->Species())) 
   {
    ep += pairEnergy(nn.r);
    pf = PairForce(nn.rij);
    mb = ManyBodies(nn.rij,rho[i],rho[nn.j->Index()]);
    acci = sc[i].Acceleration();
    accj = nn.j->Acceleration(); 
    sc.SetAcceleration(i, acci + (pf+mb)*(FORCEFACTOR/sc[i].Mass()));
    Atom * jpointer = const_cast<Atom *>(nn.j);                         // FIXME esto no se debe hacer :)
    jpointer->SetAccel(accj - (pf+mb)*(FORCEFACTOR/nn.j->Mass()));    // FIXME y menos esto :D
    tmpvir -= Dot(nn.rij, pf+mb);
    //FIXME : Corregir - Aclarar signo del stress
    for (int k=0;k<3;k++)
    {
     stress[0][k] += -(nn.rij).GetX()*(pf+mb).Get(k);
     stress[1][k] += -(nn.rij).GetY()*(pf+mb).Get(k);
     stress[2][k] += -(nn.rij).GetZ()*(pf+mb).Get(k);
    }
   }
  }
 }
 //Calcula de la Energia de Muchos Cuerpos.
#ifdef _OPENMP
#pragma omp parallel for private ( i ) reduction ( + : ep )
#endif
 for (i=0;i<n;++i) {ep += F(rho[i]);}
 sc.MetaData().AssignParameter("pe", ToString<double>(ep));
 sc.AddToVirial(tmpvir);
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

