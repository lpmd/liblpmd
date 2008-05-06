//
//
//

#ifndef __LPMD_TWOSTEP_INTEGRATOR_H__
#define __LPMD_TWOSTEP_INTEGRATOR_H__

#include <lpmd/integrator.h>

namespace lpmd
{

class TwoStepIntegrator: public Integrator
{
 public:
   // Overloaded from Integrator
   void Advance(SimulationCell & sc, Potential & p);

   // To implement
   virtual void AdvancePosition(SimulationCell & sc, long i) = 0;
   virtual void AdvanceVelocity(SimulationCell & sc, long i) = 0;
};

} // lpmd

#endif


