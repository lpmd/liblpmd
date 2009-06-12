//
//
//

#ifndef __LPMD_SYSTEMFILTER_H__
#define __LPMD_SYSTEMFILTER_H__

#include <lpmd/systemmodifier.h>
#include <lpmd/atomselector.h>

namespace lpmd
{
 class SystemFilter: public SystemModifier
 {
  public:
   //
   SystemFilter();
   virtual ~SystemFilter();

   void Apply(Simulation & sim);
   virtual Selector<BasicParticleSet> & CreateSelector() = 0;

   bool inverted;
 };

} // lpmd 

#endif


