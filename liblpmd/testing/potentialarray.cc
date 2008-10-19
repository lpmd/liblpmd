//
//
//

#include <lpmd/potentialarray.h>
#include <lpmd/simulationcell.h>

#include <list>
#include <string>
#include <cassert>

using namespace lpmd;

class lpmd::PotArrayImpl
{
 public:
  std::list<Potential *> potlist; 
  bool initialized;

  PotArrayImpl(): initialized(false) { }
};

//
//
//

PotentialArray::PotentialArray() { impl = new PotArrayImpl(); }

PotentialArray::PotentialArray(const PotentialArray & pa)
{
 impl = new PotArrayImpl();
 impl->initialized = pa.impl->initialized;
 for (std::list<Potential *>::const_iterator it=pa.impl->potlist.begin();it != pa.impl->potlist.end();++it) 
     impl->potlist.push_back((*it));
}

PotentialArray::~PotentialArray() { delete impl; }

PotentialArray & PotentialArray::operator=(const PotentialArray & pa)
{
 if (&pa != this)
 {
  impl->initialized = pa.impl->initialized;
  impl->potlist.clear();
  for (std::list<Potential *>::const_iterator it=pa.impl->potlist.begin();it != pa.impl->potlist.end();++it) 
     impl->potlist.push_back((*it));
 }
 return (*this);
}

void PotentialArray::Set(std::string s1, std::string s2, Potential & pot)
{
 pot.SetValidSpecies(ElemNum(s1), ElemNum(s2));
 impl->potlist.push_back(&pot);
}

void PotentialArray::Set(int s1, int s2, Potential & pot)
{
 pot.SetValidSpecies(s1, s2);
 impl->potlist.push_back(&pot);
}

Potential & PotentialArray::Get(int s1, int s2)
{
 for (std::list<Potential *>::const_iterator it=impl->potlist.begin();it != impl->potlist.end();++it) 
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
 for (std::list<Potential *>::const_iterator it=impl->potlist.begin();it != impl->potlist.end();++it) (*it)->Initialize(sc);
 impl->initialized = true;
}

double PotentialArray::energy(SimulationCell & sc) 
{
 if (! impl->initialized) Initialize(sc);
 //
 // Calcula la energia para la celda, sumando la contribucion de cada potencial
 //
 double e = 0.0e0;
 for (std::list<Potential *>::const_iterator it=impl->potlist.begin();it != impl->potlist.end();++it)
 {
  Potential * p = (*it);
  e += (p->energy(sc));
 }
 return e;
}

void PotentialArray::UpdateForces(SimulationCell & sc)
{
 if (! impl->initialized) Initialize(sc);
 //
 // Actualiza las fuerzas para la celda, aplicando cada potencial secuencialmente (por ahora)
 //
 sc.MetaData().Remove("pe");
 sc.ClearForces();
 for (std::list<Potential *>::const_iterator it=impl->potlist.begin();it != impl->potlist.end();++it)
 {
  Potential * p = (*it);
  p->UpdateForces(sc);
 }
}


