//
//
//

#ifndef __LPMD_CELLMANAGER_H__
#define __LPMD_CELLMANAGER_H__

#include <lpmd/atompair.h>
#include <lpmd/array.h>

namespace lpmd
{
 class Configuration; // forward

 class CellManager
 {
  public:
   //
   CellManager();
   virtual ~CellManager();

   virtual double Cutoff() const;

   virtual void Reset() = 0;
   virtual void UpdateCell(Configuration & conf) = 0;
   virtual void UpdateAtom(Configuration & conf, long i) = 0;
   virtual void BuildNeighborList(Configuration & conf, long i, NeighborList & nlist, bool full=true, double rcut=0.0e0) = 0;   
 };

} // lpmd 

#endif


