//
//
//

#ifndef __LPMD_CELLGENERATOR_H__
#define __LPMD_CELLGENERATOR_H__

#include "simulationcell.h"

namespace lpmd
{

class CellGenerator
{
 public:
  //
  CellGenerator();
  virtual ~CellGenerator();

  virtual void Generate(SimulationCell & sc) const;
};

} // lpmd

#endif


