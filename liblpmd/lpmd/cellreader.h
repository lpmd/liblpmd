//
//
//

#ifndef __LPMD_CELLREADER_H__
#define __LPMD_CELLREADER_H__

#include "cellgenerator.h" 
#include "simulationcell.h"

#include <iostream>

namespace lpmd
{

class CellReader: public CellGenerator
{
 public:
     //
     void Generate(SimulationCell & sc) const;

     // ReadHeader y ReadCell implementan la lectura del archivo y Generate simplemente los llama
     virtual void ReadHeader(std::istream & is) const = 0;
     virtual void ReadCell(std::istream & is, SimulationCell & sc) const = 0;

     void Read(const std::string & filename, SimulationCell & sc) const;

 protected:
     std::string readfile;
};

} // lpmd 

#endif

