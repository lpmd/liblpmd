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
   void Advance(BasicParticleSet & atoms, BasicCell & cell, Potential & p);

   virtual void Advance(BasicParticleSet & atoms, BasicCell & cell, long i) = 0;
};

} // lpmd

#endif


