//
//
//

#ifndef __LPMD_CELLWRITER_H__
#define __LPMD_CELLWRITER_H__

#include <string>
#include <iostream>
#include <vector>

namespace lpmd
{

 class SimulationCell;      // forward declaration

 class CellWriter
 {
  public:
  //
   virtual ~CellWriter();

   virtual void WriteHeader(std::ostream & os, std::vector<SimulationCell> *cells=NULL) const = 0;
   virtual void WriteCell(std::ostream & os, SimulationCell & sc) const = 0;
   virtual long int GetInterval() const = 0;

   void Write(const std::string & filename, SimulationCell & sc) const;
   void WriteMany(const std::string & filename, std::vector<SimulationCell> & scs) const;

   std::string GetFile();

  protected:
   std::string writefile;
 };

} // lpmd

#endif

