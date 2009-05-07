//
//
//

#include <lpmd/session.h>
#include <lpmd/pairpotential.h>
#include <lpmd/neighbor.h>
#include <lpmd/simulationcell.h>

using namespace lpmd;

PairPotential::PairPotential() { }

PairPotential::~PairPotential() { }

double PairPotential::energy(SimulationCell & sc) { return energycache; } 

void PairPotential::UpdateForces(SimulationCell & sc)
{
// std::cerr << "DEBUG In UpdateForces: before BuildNeighborList\n";
// std::cerr << "      i: " << sc[0].Position() << '\n';
// std::cerr << "      j: " << sc[1].Position() << '\n';
 const double forcefactor = GlobalSession.GetDouble("forcefactor");
 Vector ff, acci, accj;
 const long int n = sc.size();
 energycache = 0.0;
 double tmpvir = 0.0;
 double stress[3][3];
 for (int i=0;i<3;i++)
 {
  for (int j=0;j<3;j++) stress[i][j]=0.0e0;
 }
 for (long i=0;i<n;++i)
 {
  std::vector<Neighbor> nlist;
  sc.BuildNeighborList(i, nlist, false, GetCutoff());
  for (unsigned long int k=0;k<nlist.size();++k)
  {
   Neighbor & nn = nlist[k];
   if (AppliesTo(sc[i].Z(), nn.j->Z()) && nn.r < GetCutoff()) 
   {
    if (nn.r < 0.5)
    {
     std::cerr << "DEBUG Atoms too close! r=" << nn.r << '\n';
     //FIXME : No muestra la info de los átomos involucrados.
//     std::cerr << "i: " << sc[i].Position() << '\n';
//     std::cerr << "j: " << nn.j->Position() << '\n';
//     std::cerr << "DEBUG i=" << i << " j=" << nn.j->Index() << '\n';
     exit(1);
    }
    energycache += pairEnergy(nn.r);
    ff = pairForce(nn.rij);
    sc[i].Acceleration() = acci; 
    (nn.j)->Acceleration() = accj; 
    sc.SetAcceleration(i, acci + ff*(forcefactor/sc[i].Mass()));
    Atom * jpointer = const_cast<Atom *>(nn.j);                  // esto no se debe hacer :)
    jpointer->Acceleration() = (accj - ff*(forcefactor/nn.j->Mass()));    // y menos esto :D
    tmpvir -= Dot(nn.rij, ff); // virial de pares
    if (ff.Module() > 10.0) throw HorrendousForce(ff.Module()); // DEBUG

    //Asignacion de stress, un for adicional pequeno, 
    //sera mas lento? - El signo parec provenir de la fuerza, ojo con eso
    for (int k=0;k<3;k++)
    {
     stress[0][k] += -(nn.rij)[0]*ff[k];
     stress[1][k] += -(nn.rij)[1]*ff[k];
     stress[2][k] += -(nn.rij)[2]*ff[k];
    }
   }
  }
 }
 sc.AddToVirial(tmpvir);
 for (int i=0;i<3;i++)
 {
  sc.StressTensor(0,i) = stress[0][i];
  sc.StressTensor(1,i) = stress[1][i];
  sc.StressTensor(2,i) = stress[2][i];
 }
}


