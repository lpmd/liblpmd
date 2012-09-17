//
//
//

#ifndef __LPMD_VISUALIZER_H__
#define __LPMD_VISUALIZER_H__

#include <lpmd/stepper.h>

namespace lpmd
{
 class Simulation; // forward

 class Visualizer: public Stepper
 {
  public:
   //
   Visualizer();
   virtual ~Visualizer();

   virtual void Apply(const Simulation & md) = 0;
 };

} // lpmd

#endif


