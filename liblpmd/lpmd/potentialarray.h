//
//
//

#ifndef __LPMD_POTENTIALARRAY_H__
#define __LPMD_POTENTIALARRAY_H__

#include "potential.h"
#include <list>

namespace lpmd
{

class PotentialArray: public Potential
{
 public:
   //    
   void Set(std::string s1, std::string s2, Potential & p);
   void Set(int s1, int s2, Potential & p);

   double energy(SimulationCell & sc) ;
   void UpdateForces(SimulationCell & sc);

 private:
   std::list<Potential *> potlist;
};

} // lpmd

#endif


