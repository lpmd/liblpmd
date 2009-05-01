//
//
//

#ifndef __LPMD_SYSTEMMODIFIER_H__
#define __LPMD_SYSTEMMODIFIER_H__

#include <lpmd/stepper.h>

namespace lpmd
{

 class MD;             // forward declaration
 class SimulationCell; // 

 class SystemModifier: public Stepper
 {
  public:
   //
   SystemModifier();
   virtual ~SystemModifier();

   virtual void Apply(SimulationCell & sc);
   virtual void Apply(MD & md) = 0;
 };

} // lpmd 

#endif


