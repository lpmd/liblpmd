//
//
//

#ifndef __LPMD_CELLGENERATOR_H__
#define __LPMD_CELLGENERATOR_H__

namespace lpmd
{

 class SimulationCell;     // forward declaration

 class CellGenerator
 {
  public:
   //
   CellGenerator();
   virtual ~CellGenerator();
 
   virtual void Generate(SimulationCell & sc) const;
 };

} // lpmd

#endif


