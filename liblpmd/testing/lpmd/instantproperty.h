//
//
//

#ifndef __LPMD_INSTANTPROPERTY_H__
#define __LPMD_INSTANTPROPERTY_H__

#include <lpmd/stepper.h>

#include <string>

namespace lpmd
{

 class SimulationCell;               // forward declaration
 class Potential;                    // forward declaration

 class InstantProperty: public Stepper
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


