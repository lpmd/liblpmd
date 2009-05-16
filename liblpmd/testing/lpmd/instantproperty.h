//
//
//

#ifndef __LPMD_INSTANTPROPERTY_H__
#define __LPMD_INSTANTPROPERTY_H__

#include <lpmd/stepper.h>

#include <string>

namespace lpmd
{
 class Potential;        // forward
 class Configuration;    // forward

 class InstantProperty: public Stepper
 {
  public:
   //
   InstantProperty();
   virtual ~InstantProperty();

   // Metodos virtuales
   virtual void Evaluate(Configuration & conf, Potential & pot);

   //
   std::string outputfile;
 };

} // lpmd

#endif

