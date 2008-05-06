//
//
//

#ifndef __LPMD_INSTANTPROPERTY_H__
#define __LPMD_INSTANTPROPERTY_H__

#include <lpmd/applicable.h>

#include <string>

namespace lpmd
{

 class SimulationCell;               // forward declaration
 class Potential;                    // forward declaration

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


