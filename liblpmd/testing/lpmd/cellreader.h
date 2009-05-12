//
//
//

#ifndef __LPMD_CELLREADER_H__
#define __LPMD_CELLREADER_H__

#include <lpmd/cellgenerator.h>
#include <lpmd/error.h>

#include <iostream>
#include <vector>

namespace lpmd
{

 class InvalidInput: public Error
 {
  public:
   InvalidInput(const std::string & filename); 
 };

 class BasicParticleSet;     // forward declaration
 class BasicCell;      // forward declaration

 class CellReader: public CellGenerator
 {
  public:
      //
      void Generate(BasicParticleSet & atoms, BasicCell & cell) const;
 
      // ReadHeader y ReadCell implementan la lectura del archivo y Generate simplemente los llama
      virtual void ReadHeader(std::istream & is) const = 0;
      virtual bool ReadCell(std::istream & is, BasicParticleSet & atoms, BasicCell & cell) const = 0;
 
      void Read(const std::string & filename, BasicParticleSet & atoms, BasicCell & cell) const;
//      void ReadMany(const std::string & filename, std::vector<SimulationCell> & scs) const;

  protected:
      std::string readfile;
 };

} // lpmd 

#endif

