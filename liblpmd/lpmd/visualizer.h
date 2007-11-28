//
//
//

#ifndef __LPMD_VISUALIZER_H__
#define __LPMD_VISUALIZER_H__

#include "applicable.h"
#include "md.h"

namespace lpmd
{

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


