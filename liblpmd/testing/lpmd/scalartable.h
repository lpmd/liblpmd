//
//
//

#ifndef __LPMD_SCALARTABLE_H__
#define __LPMD_SCALARTABLE_H__

#include <lpmd/containable.h>
#include <lpmd/matrix.h>

namespace lpmd
{

class ScalarTable: public IContainable
{
 public:
   ScalarTable();
   virtual ~ScalarTable();

   // Implementaciones de metodos de IContainable
   void ClearAverage();
   void AddToAverage();
   void OutputTo(std::ostream & os) const;
   void OutputAverageTo(std::ostream & os) const;

   // Metodos virtuales
   virtual const Matrix & Value() const = 0;

 private:
   Matrix v_av;
   long n;
};

} // lpmd

#endif


