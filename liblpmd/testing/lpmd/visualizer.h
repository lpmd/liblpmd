//
//
//

#ifndef __LPMD_VISUALIZER_H__
#define __LPMD_VISUALIZER_H__

#include <lpmd/applicable.h>

namespace lpmd
{

 class MD;             // forward declaration

 class Visualizer: public IApplicable
 {
  public:
   //
   Visualizer();
   virtual ~Visualizer();

   virtual void Apply(const MD & md) = 0;
 };

} // lpmd

#endif


