//
//
//

#ifndef __LPMD_VISUALIZER_H__
#define __LPMD_VISUALIZER_H__

#include <lpmd/stepper.h>

namespace lpmd
{

 class MD;             // forward declaration

 class Visualizer: public Stepper
 {
  public:
   //
   Visualizer();
   virtual ~Visualizer();

   virtual void Apply(const MD & md) = 0;
 };

} // lpmd

#endif


