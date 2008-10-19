//
//
//

#ifndef __LPMD_POTENTIALARRAY_H__
#define __LPMD_POTENTIALARRAY_H__

//#include <list>

#include <lpmd/potential.h>

namespace lpmd
{

class PotentialArray: public Potential
{
 public:
   //
   PotentialArray();
   PotentialArray(const PotentialArray & pa);
   virtual ~PotentialArray();
   
   //    
   PotentialArray & operator=(const PotentialArray & pa);
   void Set(std::string s1, std::string s2, Potential & p);
   void Set(int s1, int s2, Potential & p);
   Potential & Get(int s1, int s2);
   Potential & Get(std::string s1, std::string s2);

   void Initialize(SimulationCell & sc);
   double energy(SimulationCell & sc);
   void UpdateForces(SimulationCell & sc);

 private:
   class PotArrayImpl * impl;
};

} // lpmd

#endif


