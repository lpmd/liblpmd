//
//
//

#ifndef __LPMD_CELLWRITER_H__
#define __LPMD_CELLWRITER_H__

#include <string>
#include <iostream>

namespace lpmd
{
 class SimulationHistory; // forward
 class Configuration;     // forward

 class CellWriter
 {
  public:
   virtual ~CellWriter();

   virtual void WriteHeader(std::ostream & os, SimulationHistory * hist = 0) const = 0;
   virtual void WriteCell(std::ostream & os, Configuration & conf) const = 0;
   virtual long int GetInterval() const = 0;

   void Write(const std::string & filename, Configuration & conf) const;
   void WriteMany(const std::string & filename, SimulationHistory & scs) const;

   std::string GetFile();

  protected:
   std::string writefile;
 };

} // lpmd

#endif

