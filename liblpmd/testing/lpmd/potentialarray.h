//
//
//

#ifndef __LPMD_POTENTIALARRAY_H__
#define __LPMD_POTENTIALARRAY_H__

#include <list>

#include <lpmd/potential.h>

namespace lpmd
{

class PotentialArray: public Potential
{
 public:
   //
   PotentialArray();
   
   //    
   void Set(std::string s1, std::string s2, Potential & p);
   void Set(int s1, int s2, Potential & p);

   void Initialize(SimulationCell & sc);
   double energy(SimulationCell & sc);
   void UpdateForces(SimulationCell & sc);

 private:
   std::list<Potential *> potlist;
   bool initialized;
};

} // lpmd

#endif


