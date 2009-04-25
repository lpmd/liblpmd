//
//
//

#ifndef __LPMD_VALUE_H__
#define __LPMD_VALUE_H__

#include <lpmd/containable.h>

namespace lpmd
{

template <typename T> class Value: public IContainable
{
 public:
   Value() { ClearAverage(); }
   virtual ~Value() { }

   // Implementaciones de metodos de IContainable
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

   // Metodos virtuales
   virtual const T & CurrentValue() const = 0;

 private:
   T v_av;
   long n;
};

} // lpmd

#endif

