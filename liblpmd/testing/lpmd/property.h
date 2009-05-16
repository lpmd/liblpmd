/* 
 *
 *
 *
 */

#ifndef __LPMD_PROPERTY_H__
#define __LPMD_PROPERTY_H__

#include <lpmd/stepper.h>
#include <string>

namespace lpmd
{
 class Potential; // forward
 class Configuration; // forward
 class SimulationHistory; // forward

 template <typename T> class Property
 {
  public:
    virtual ~Property();

    virtual void Evaluate(const T & target, Potential & p) = 0;


 };


 template <> class Property<Configuration>: public Stepper 
 { 
  public:
    std::string & OutputFile() { return ofile; };

  private:
    std::string ofile;
 };

 template <> class Property<SimulationHistory> { };

 typedef Property<Configuration> InstantProperty;
 typedef Property<SimulationHistory> TemporalProperty;


}  // lpmd

#endif

