//
//
//

#ifndef __LPMD_TWOSTEP_INTEGRATOR_H__
#define __LPMD_TWOSTEP_INTEGRATOR_H__

#include "integrator.h"

namespace lpmd
{

class TwoStepIntegrator: public Integrator
{
 public:
   // Overloaded from Integrator
   void Advance(SimulationCell & sc, Potential & p);

   // To implement
   virtual void AdvancePositions(SimulationCell & sc) = 0;
   virtual void AdvanceVelocities(SimulationCell & sc) = 0;
};

} // lpmd

#endif


