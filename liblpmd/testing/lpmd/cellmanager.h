//
//
//

#ifndef __LPMD_CELLMANAGER_H__
#define __LPMD_CELLMANAGER_H__

#include <list>
#include <lpmd/neighbor.h>

namespace lpmd
{
 class SimulationCell;  // forward declaration

 class CellManager
 {
  public:
   //
   CellManager();
   virtual ~CellManager();

   virtual void Reset() = 0;
   virtual void UpdateCell(SimulationCell & sc) = 0;
   virtual void BuildNeighborList(SimulationCell & sc, long i, std::list<Neighbor> & nlist, bool full=true) = 0;
 };

} // lpmd 

#endif

