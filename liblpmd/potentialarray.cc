//
//
//

#include <lpmd/potentialarray.h>
#include <lpmd/module.h>           // FIXME: quitar pronto

using namespace lpmd;

void PotentialArray::Set(std::string s1, std::string s2, Potential & pot)
{
 pot.SetValidSpecies(ElemNum(s1), ElemNum(s2));
 potlist.push_back(&pot);
}

void PotentialArray::Set(int s1, int s2, Potential & pot)
{
 pot.SetValidSpecies(s1, s2);
 potlist.push_back(&pot);
}

double PotentialArray::energy(SimulationCell & sc) 
{
 //
 // Calcula la energia para la celda, sumando la contribucion de cada potencial
 //
 double e = 0.0e0;
 for (std::list<Potential *>::const_iterator it=potlist.begin();it != potlist.end();++it)
 {
  Potential * p = (*it);
  e += (p->energy(sc));
 }
 return e;
}

void PotentialArray::UpdateForces(SimulationCell & sc)
{
 //
 // Actualiza las fuerzas para la celda, aplicando cada potencial secuencialmente (por ahora)
 //
 // std::cerr << "\n\nDEBUG Comienza PotentialArray::UpdateForces\n";
 sc.ClearForces();
 long n = sc.Size();
 for (std::list<Potential *>::const_iterator it=potlist.begin();it != potlist.end();++it)
 {
  Potential * p = (*it);
  Module & mod = dynamic_cast<Module &>(*p);
  // std::cerr << "DEBUG Antes de aplicar potencial " << mod.Name() << '\n';
  // std::cerr << "DEBUG acci: " << sc[(int)(n/2)].Acceleration() << '\n';
  p->UpdateForces(sc);
  // std::cerr << "DEBUG Despues de aplicar potencial " << mod.Name() << '\n';
  // std::cerr << "DEBUG acci: " << sc[(int)(n/2)].Acceleration() << '\n';
 }
 // std::cerr << "\nDEBUG Termina PotentialArray::UpdateForces\n";
}


