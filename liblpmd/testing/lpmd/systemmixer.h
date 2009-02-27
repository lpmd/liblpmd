//
//
//

#ifndef __LPMD_SYSTEMMIXER_H__
#define __LPMD_SYSTEMMIXER_H__

namespace lpmd
{

 class SimulationCell; // forward declaration 

 class SystemMixer
 {
  public:
   //
   SystemMixer();
   virtual ~SystemMixer();

   virtual SimulationCell Apply(SimulationCell & sc1, SimulationCell & sc2) = 0;
 };

} // lpmd 

#endif


