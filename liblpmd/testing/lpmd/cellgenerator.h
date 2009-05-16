//
//
//

#ifndef __LPMD_CELLGENERATOR_H__
#define __LPMD_CELLGENERATOR_H__

namespace lpmd
{
 class Configuration;  // forward

 class CellGenerator
 {
  public:
   CellGenerator();
   virtual ~CellGenerator();
 
   virtual void Generate(Configuration & conf) const;
 };

} // lpmd

#endif


