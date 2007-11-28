//
//
//

#ifndef __LPMD_INSTANTPROPERTY_H__
#define __LPMD_INSTANTPROPERTY_H__

#include "applicable.h"
#include "simulationcell.h"
#include "potential.h"

namespace lpmd
{

class InstantProperty: public IApplicable
{
 public:
  //
  InstantProperty();
  virtual ~InstantProperty();

  // Metodos virtuales
  virtual void Evaluate(SimulationCell & simcell, Potential & pot);

  //
  std::string outputfile;
};

} // lpmd

#endif


