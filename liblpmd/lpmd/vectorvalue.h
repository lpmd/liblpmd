//
//
//

#ifndef __LPMD_VECTORVALUE_H__
#define __LPMD_VECTORVALUE_H__

#include "containable.h"
#include "vector.h"

namespace lpmd
{

class VectorValue: public IContainable
{
 public:
   VectorValue();
   virtual ~VectorValue();

   // Implementaciones de metodos de IContainable
   void ClearAverage();
   void AddToAverage();
   void OutputTo(std::ostream & os) const;
   void OutputAverageTo(std::ostream & os) const;

   // Metodos virtuales
   virtual const Vector & Value() const = 0;

 private:
    Vector v_av;
    long n;
};

} // lpmd

#endif


