//
//
//

#ifndef __LPMD_TEMPORALPROPERTY_H__
#define __LPMD_TEMPORALPROPERTY_H__

namespace lpmd
{
 class SimulationHistory;  // forward
 class Potential;          // forward

 class TemporalProperty
 {
  public:
   TemporalProperty();
   TemporalProperty(const SimulationHistory & hist, Potential & pot);
   virtual ~TemporalProperty();

   virtual void Evaluate(const SimulationHistory & hist, Potential & pot);
 };

} // lpmd

#endif


