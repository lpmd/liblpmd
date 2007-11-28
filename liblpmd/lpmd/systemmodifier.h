//
//
//

#ifndef __LPMD_SYSTEMMODIFIER_H__
#define __LPMD_SYSTEMMODIFIER_H__

#include "applicable.h"
#include "md.h"

namespace lpmd
{

class SystemModifier: public IApplicable
{
 public:
  //
  SystemModifier();
  virtual ~SystemModifier();

  virtual void Apply(MD & md) = 0;
};

} // lpmd 

#endif


