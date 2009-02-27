//
//
//

#include <lpmd/potentialarray.h>
#include <lpmd/simulationcell.h>

#include <string>
#include <cassert>

using namespace lpmd;

PotentialArray::PotentialArray(): initialized(false) { }

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

Potential & PotentialArray::Get(int s1, int s2)
{
 for (std::list<Potential *>::const_iterator it=potlist.begin();it != potlist.end();++it) 
 {
  if ((*it)->AppliesTo(s1, s2)) return *(*it);
 }
 assert(false); // No hay potential que aplique a s1 y s2 en este PotentialArray
}

Potential & PotentialArray::Get(std::string s1, std::string s2)
{
 return PotentialArray::Get(ElemNum(s1), ElemNum(s2));
}

void PotentialArray::Initialize(SimulationCell & sc)
{
 for (std::list<Potential *>::const_iterator it=potlist.begin();it != potlist.end();++it) (*it)->Initialize(sc);
 initialized = true;
}

double PotentialArray::energy(SimulationCell & sc) 
{
 if (! initialized) Initialize(sc);
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
 if (! initialized) Initialize(sc);
 //
 // Actualiza las fuerzas para la celda, aplicando cada potencial secuencialmente (por ahora)
 //
 sc.ClearForces();
 for (std::list<Potential *>::const_iterator it=potlist.begin();it != potlist.end();++it)
 {
  Potential * p = (*it);
  p->UpdateForces(sc);
 }
}


