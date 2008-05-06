//
//
//

#ifndef __LPMD_CONTAINABLE_H__
#define __LPMD_CONTAINABLE_H__

#include <iostream>

namespace lpmd
{

class IContainable
{
 public:
   //
   IContainable();
   virtual ~IContainable();

   //
   virtual void ClearAverage() = 0;
   virtual void AddToAverage() = 0;

   virtual void OutputTo(std::ostream & os) const = 0;
   virtual void OutputAverageTo(std::ostream & os) const = 0;

};

std::ostream & operator<<(std::ostream & os, const IContainable & c);

} // lpmd

#endif


