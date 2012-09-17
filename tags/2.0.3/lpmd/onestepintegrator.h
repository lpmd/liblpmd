//
//
//

#ifndef __LPMD_ONESTEP_INTEGRATOR_H__
#define __LPMD_ONESTEP_INTEGRATOR_H__

#include <lpmd/integrator.h>

namespace lpmd
{

class OneStepIntegrator: public Integrator
{
 public:
   void Advance(Simulation & sim, Potential & p);

   virtual void Advance(Simulation & sim, long i) = 0;
};

} // lpmd

#endif


