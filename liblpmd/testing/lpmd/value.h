//
//
//

#ifndef __LPMD_CONTAINABLE_H__
#define __LPMD_CONTAINABLE_H__

#include <iostream>

namespace lpmd
{

 class AbstractValue
 {
  public:
   virtual ~AbstractValue();
   virtual void ClearAverage() = 0;
   virtual void AddToAverage() = 0;
   virtual void OutputTo(std::ostream & os) const = 0;
   virtual void OutputAverageTo(std::ostream & os) const = 0;

   void OutputToFile(const std::string & filename) const;
 };

template <typename T> class Value: public AbstractValue
{
 public:
   virtual ~Value() { };

   virtual T & CurrentValue() = 0;
   virtual const T & CurrentValue() const = 0;
};

} // lpmd

#endif

