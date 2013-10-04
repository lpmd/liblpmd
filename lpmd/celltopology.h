/*
 *
 *
 *
 */

#ifndef __TOPOLOGY_H__
#define __TOPOLOGY_H__

#include <lpmd/basiccell.h>
//#include <unistd.h>
#include "topology.h"

namespace lpmd
{

 class CellTopology: public parallel::Topology
 {
  public:
     CellTopology(const BasicCell & cell, int nx, int ny): parallel::Topology(nx, ny)
     {
      assert (cell.IsOrthogonal());
      for (int q=0;q<3;++q) _cell[q] = cell[q].Module(); // FIXME: Solo ortogonal!

      const double dx = _cell[0]/nx;
      const double dy = _cell[1]/ny;
 
      cell_min[0] = dx*(PositionAlong(0));
      cell_max[0] = dx*(1+PositionAlong(0));
      cell_min[1] = dy*(PositionAlong(1));
      cell_max[1] = dy*(1+PositionAlong(1));
      cell_min[2] = 0.0;
      cell_max[2] = _cell[2];
     }

   const double * GetCellDimensions() const { return _cell; }

   double cell_min[3], cell_max[3], _cell[3];
   
 };

} // lpmd

#endif

