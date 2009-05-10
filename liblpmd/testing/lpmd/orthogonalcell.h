/*
 *
 *
 *
 */

#ifndef __LPMD_ORTHOGONALCELL_H__
#define __LPMD_ORTHOGONALCELL_H__

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string.h>

#include <lpmd/basiccell.h>
#include <lpmd/util.h>

namespace lpmd
{

class OrthogonalCell: public BasicCell
{
 public:
  OrthogonalCell()
  {
   v[0]=e1;
   v[1]=e2;
   v[2]=e3;
   for (int q=0;q<3;++q) p[q] = true;
  }

  OrthogonalCell(const OrthogonalCell & c)
  {
   for (int q=0;q<3;++q) 
   { 
    v[q] = c[q];
    p[q] = c.p[q];
   }
  }

  OrthogonalCell(const double a, const double b, const double c)
  {
   v[0] = a*e1;
   v[1] = b*e2;
   v[2] = c*e3;
   for (int i=0;i<3;++i) p[i] = true;
  }

  bool IsOrthogonal() const { return true; }

  inline OrthogonalCell & operator=(const OrthogonalCell & c)
  {
   if (this != &c)
   {
    for (int q=0;q<3;++q) v[q] = c[q];
    for (int i=0;i<3;++i) p[i] = c.p[i];
   }
   return (*this);
  }

  inline Vector & operator[](int q)
  { 
   mustupdate = true;   
   return v[q]; 
  }  

  inline const Vector & operator[](int q) const { return v[q]; }

  inline const OrthogonalCell & operator*=(double a)
  {
   for (int i=0;i<3;++i) v[i] *= a;
   mustupdate = true;   
   return (*this);
  }

  bool & Periodicity(int i) { return p[i]; }

  const bool & Periodicity(int i) const { return p[i]; }

  Vector ScaleByCell(const BasicVector & cv) const
  {
   Vector nv;
   for (int j=0;j<3;++j) nv += v[j]*cv[j];
   return nv;
  }

  Vector FittedInside(const BasicVector & a) const
  {
   Vector vtmp(a);
   for (int q=0;q<3;++q)
   {
    if (p[q] == true)
    {
     double ll = v[q].Module();
     if (vtmp[q] < 0.0) vtmp[q] -= ll*floor(vtmp[q]/ll);
     else if (vtmp[q] > ll) vtmp[q] -= ll*floor(vtmp[q]/ll);
    }
   } 
   return vtmp;
  }

  bool IsInside(const BasicVector & a) const
  {
   Vector vtmp(a);
   for (int q=0;q<3;++q) 
     if ((vtmp[q] < 0.0) || (vtmp[q] > v[q].Module())) return false;
   return true;
  }

  Vector Displacement(const BasicVector & a, const BasicVector & b) const
  {
   Vector d = b - a;
   for (int i=0;i<3;++i)
   {
    double q = d[i];
    if (p[i] == true)
    {
     double ll = v[i].Module();
     if (q >= 0.5*ll) q -= ll;
     else if (q < -0.5*ll) q += ll;
     d[i] = q;
    }
   }
   return d;
  }

  double Volume() const 
  {
   Vector tmp = Cross(v[0], v[1]);
   return fabs(Dot(tmp, v[2]));
  }

 private:
   Vector v[3];
   bool p[3];
   mutable bool mustupdate;

};

}

#endif

