//
//
//

#ifndef __LPMD_TEMPORALPROPERTY_H__
#define __LPMD_TEMPORALPROPERTY_H__

#include <list>

#include <lpmd/simulationcell.h>

namespace lpmd
{
 
 class Potential;                   // forward declaration

 class TemporalProperty
 {
  public:
   //
   TemporalProperty();
   TemporalProperty(const std::vector<SimulationCell> & simcells, Potential & pot);
   virtual ~TemporalProperty();

   virtual void Evaluate(const std::vector<SimulationCell> & simcells, Potential & pot);
 
 };

} // lpmd

#endif


