//
//
//

#ifndef __LPMD_STOREDVALUE_H__
#define __LPMD_STOREDVALUE_H__

#include <lpmd/value.h>

namespace lpmd
{

template <typename T> class StoredValue: public Value<T>
{
 public:
   StoredValue() { ClearAverage(); }
   virtual ~StoredValue() { }

   void ClearAverage()
   { 
    v_av = T();
    n = 0;
   }

   void AddToAverage()
   {
    v_av += CurrentValue(); 
    ++n;
   }

   void OutputTo(std::ostream & os) const { os << CurrentValue(); } 

   void OutputAverageTo(std::ostream & os) const
   {
    if (n > 0) os << v_av/double(n);
    else os << T();
   }

   T & CurrentValue() { return currv; }
   const T & CurrentValue() const { return currv; }

 private:
   T v_av, currv;
   long int n;
};

} // lpmd

#endif

