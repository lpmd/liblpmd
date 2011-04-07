//
//
//

#ifndef __LPMD_CELLREADER_H__
#define __LPMD_CELLREADER_H__

#include <lpmd/cellgenerator.h>
#include <lpmd/stepper.h>

#include <iostream>

namespace lpmd
{
 class SimulationHistory; // forward
 class Configuration;     // forward

 class CellReader: public CellGenerator
 {
  public:
      void Generate(Configuration & conf) const;
 
      virtual void ReadHeader(std::istream & is) const = 0;
      virtual bool ReadCell(std::istream & is, Configuration & conf) const = 0;
      virtual bool SkipCell(std::istream & is) const;
 
      void Read(const std::string & filename, Configuration & conf) const;
      void ReadMany(const std::string & filename, SimulationHistory & hist, const Stepper & stepper, bool skipheader=false) const;
      void ReadMany(std::istream & inputstream, SimulationHistory & hist, const Stepper & stepper, bool skipheader=false) const;

  protected:
      std::string readfile;
 };

} // lpmd 

#endif

