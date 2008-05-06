//
//
//

#ifndef __LPMD_COULOMB_H__
#define __LPMD_COULOMB_H__

#include <lpmd/pairpotential.h>

namespace lpmd
{

class Coulomb: public PairPotential
{
 public:
   // Constructor
   Coulomb(double Q1, double Q2);
   Coulomb();
   virtual ~Coulomb();

   double pairEnergy(const double & r) const;

   Vector pairForce(const Vector & r) const;

   void SetValue1(double Q1);
   void SetValue2(double Q2);

   double GetValue1() const {return q1;};
   double GetValue2() const {return q2;};

private:
 double q1, q2;
};

} // lpmd

#endif


