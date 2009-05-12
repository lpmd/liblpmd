//
//
//

#ifndef __LPMD_CELLGENERATOR_H__
#define __LPMD_CELLGENERATOR_H__

#include <lpmd/basiccell.h>
#include <lpmd/basicparticleset.h>

namespace lpmd
{
 class CellGenerator
 {
  public:
   //
   CellGenerator();
   virtual ~CellGenerator();
 
   virtual void Generate(BasicParticleSet & atoms, BasicCell & cell) const;
 };

} // lpmd

#endif


