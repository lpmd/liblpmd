//
//
//

#ifndef __LPMD_CELLMANAGER_H__
#define __LPMD_CELLMANAGER_H__

#include <lpmd/array.h>
#include <lpmd/neighbor.h>
#include <lpmd/basiccell.h>
#include <lpmd/basicparticleset.h>

namespace lpmd
{

 class CellManager
 {
  public:
   //
   CellManager();
   virtual ~CellManager();

   virtual double Cutoff() const;

   virtual void Reset() = 0;
   virtual void UpdateCell(BasicParticleSet & atoms, BasicCell & cell) = 0;
   virtual void BuildNeighborList(BasicParticleSet & atoms, BasicCell & cell, long i, Array<Neighbor> & nlist, bool full=true, double rcut=0.0e0) = 0;   
 };

} // lpmd 

#endif


