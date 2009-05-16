//
//
//

#ifndef __LPMD_CELLREADER_H__
#define __LPMD_CELLREADER_H__

#include <lpmd/cellgenerator.h>
#include <lpmd/error.h>

#include <iostream>

namespace lpmd
{
 class InvalidInput: public Error
 {
  public:
   InvalidInput(const std::string & filename); 
 };

 class SimulationHistory; // forward
 class Configuration;     // forward

 class CellReader: public CellGenerator
 {
  public:
      void Generate(Configuration & conf) const;
 
      // ReadHeader y ReadCell implementan la lectura del archivo y Generate simplemente los llama
      virtual void ReadHeader(std::istream & is) const = 0;
      virtual bool ReadCell(std::istream & is, Configuration & conf) const = 0;
 
      void Read(const std::string & filename, Configuration & conf) const;
      void ReadMany(const std::string & filename, SimulationHistory & scs) const;

  protected:
      std::string readfile;
 };

} // lpmd 

#endif

