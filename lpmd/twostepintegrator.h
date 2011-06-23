//
//
//

#ifndef __LPMD_TWOSTEP_INTEGRATOR_H__
#define __LPMD_TWOSTEP_INTEGRATOR_H__

#include <lpmd/integrator.h>
#include <lpmd/simulation.h>

namespace lpmd
{

class TwoStepIntegrator: public Integrator
{
 public:
   // Overloaded from Integrator
   void Advance(Simulation & sim, Potential & p);

   // To implement
   virtual void AdvancePosition(Simulation & sim, long i) = 0;
   virtual void AdvanceVelocity(Simulation & sim, long i) = 0;
};

} // lpmd

#endif


