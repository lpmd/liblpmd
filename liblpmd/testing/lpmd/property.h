/* 
 *
 *
 *
 */

#ifndef __LPMD_PROPERTY_H__
#define __LPMD_PROPERTY_H__

#include <lpmd/stepper.h>
#include <lpmd/configuration.h>
#include <lpmd/simulationhistory.h>
#include <lpmd/potential.h>

#include <string>

namespace lpmd
{
 template <typename T> class Property
 {
  public:
    virtual ~Property() { };

    virtual void Evaluate(T & target, lpmd::Potential & p) = 0;
 };

 class InstantProperty: public lpmd::Property<lpmd::Configuration>, public lpmd::Stepper
 {
  public:
    virtual ~InstantProperty();

    std::string & OutputFile();

  private:
    std::string ofile;
 };

 typedef lpmd::Property<lpmd::SimulationHistory> TemporalProperty;

}  // lpmd

#endif

