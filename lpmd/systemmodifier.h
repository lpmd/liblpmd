//
//
//

#ifndef __LPMD_SYSTEMMODIFIER_H__
#define __LPMD_SYSTEMMODIFIER_H__

#include <lpmd/stepper.h>
#include <lpmd/simulation.h>

namespace lpmd
{
 class SystemModifier: public Stepper
 {
  public:
   //
   SystemModifier();
   virtual ~SystemModifier();

   virtual void Apply(Simulation & sim) = 0;
 };

} // lpmd 

#endif


