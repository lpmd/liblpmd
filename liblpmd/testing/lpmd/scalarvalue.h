//
//
//

#ifndef __LPMD_SCALARVALUE_H__
#define __LPMD_SCALARVALUE_H__

#include <lpmd/containable.h>

namespace lpmd
{

class ScalarValue: public IContainable
{
 public:
   ScalarValue();
   virtual ~ScalarValue();

   // Implementaciones de metodos de IContainable
   void ClearAverage();
   void AddToAverage();
   void OutputTo(std::ostream & os) const;
   void OutputAverageTo(std::ostream & os) const;

   // Metodos virtuales
   virtual const double & Value() const = 0;

 private:
   double v_av;
   long n;
};

} // lpmd

#endif


