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
   for (int q=0;q<3;++q) { modv[q] = 1.0; p[q] = true; }
  }

  OrthogonalCell(const OrthogonalCell & c)
  {
   for (int q=0;q<3;++q) 
   { 
    v[q] = c[q];
    modv[q] = v[q][q];
    p[q] = c.p[q];
   }
  }

  OrthogonalCell(const double a, const double b, const double c)
  {
   v[0] = a*e1;
   v[1] = b*e2;
   v[2] = c*e3;
   for (int i=0;i<3;++i) { p[i] = true; modv[i] = v[i][i]; }
  }

  bool IsOrthogonal() const { return true; }

  inline OrthogonalCell & operator=(const OrthogonalCell & c)
  {
   if (this != &c)
   {
    for (int q=0;q<3;++q) { v[q] = c[q]; modv[q] = v[q][q]; }
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
   for (int i=0;i<3;++i) { v[i] *= a; }
   UpdateInternals();
   return (*this);
  }

  bool & Periodicity(int i) { return p[i]; }

  const bool & Periodicity(int i) const { return p[i]; }

  Vector ScaleByCell(const Vector & cv) const
  {
   Vector nv;
   for (int j=0;j<3;++j) nv += v[j]*cv[j];
   return nv;
  }

  inline Vector Fractional(const Vector & v) const 
  { 
   if (mustupdate) UpdateInternals();
   return Vector(v[0]/modv[0], v[1]/modv[1], v[2]/modv[2]); 
  }
  
  inline Vector Cartesian(const Vector & v) const 
  { 
   if (mustupdate) UpdateInternals();
   return Vector(v[0]*modv[0], v[1]*modv[1], v[2]*modv[2]); 
  }

  Vector FittedInside(const Vector & a) const
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

  bool IsInside(const Vector & a) const
  {
   Vector vtmp(a);
   for (int q=0;q<3;++q) 
     if ((vtmp[q] < 0.0) || (vtmp[q] > v[q].Module())) return false;
   return true;
  }

  void UpdateInternals() const
  {
   for (int q=0;q<3;++q) { modv[q] = v[q][q]; }
   mustupdate = false;
  }

  Vector Displacement(const Vector & a, const Vector & b) const
  {
   if (mustupdate) UpdateInternals();
   double m, mhalf, d[3], * dd;
   for (int q=0;q<3;++q) d[q] = b[q]-a[q];
   for (int i=0;i<3;++i)
   {
    if (!p[i]) continue;
    dd = &d[i];
    m = modv[i];
    mhalf = 0.5*m;
    if (*dd >= mhalf) (*dd) -= m;
    else if (*dd < -mhalf) (*dd) += m;
   }
   return Vector(d);
  }

  void FixDisplacement(Vector & delta)
  {
   if (mustupdate) UpdateInternals();
   double m, mhalf, *dd;
   for (int i=0;i<3;++i)
   {
    if (!p[i]) continue;
    m = modv[i];
    dd = &(delta[i]);
    mhalf = 0.5*m;
    if (*dd >= mhalf) (*dd) -= m;
    else if (*dd < -mhalf) (*dd) += m;
   }
  }

  double Volume() const 
  {
   Vector tmp = Cross(v[0], v[1]);
   return fabs(Dot(tmp, v[2]));
  }

 private:
   Vector v[3];
   bool p[3];
   mutable double modv[3];
   mutable bool mustupdate;
};

}

#endif

