/*
 *
 *
 *
 */

#ifndef __LPMD_SOLVER_H__
#define __LPMD_SOLVER_H__

#include <lpmd/systemmodifier.h>

namespace lpmd
{
 class Solver: public SystemModifier
 {
  public:

   inline void Apply(Simulation & sim) { Advance(sim); }

   virtual void Advance(Simulation & sim) = 0;
 };

}  // lpmd

#endif

