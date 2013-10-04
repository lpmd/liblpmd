/*
 *
 *
 *
 */

#ifndef __LPMD_NONORTHOGONALCELL_H__
#define __LPMD_NONORTHOGONALCELL_H__

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string.h>

#include <lpmd/basiccell.h>
#include <lpmd/util.h>

inline double DegreesToRadians(double d) { return (M_PI*d/180.0); }

namespace lpmd
{

class NonOrthogonalCell: public BasicCell
{
 public:
  NonOrthogonalCell();

  NonOrthogonalCell(const BasicCell & c);

  NonOrthogonalCell(const NonOrthogonalCell & c);

  NonOrthogonalCell(const Vector & a, const Vector & b, const Vector & c);

  NonOrthogonalCell(const double a, const double b, const double c, const double alpha, const double beta, const double gamma);

  bool IsOrthogonal() const { return !(nonortg); }

  inline NonOrthogonalCell & operator=(const NonOrthogonalCell & c)
  {
   if (this != &c)
   {
    for (int q=0;q<3;++q) v[q] = c[q];
    for (int i=0;i<3;++i) p[i] = c.p[i];
    UpdateTransfMatrix();
   }
   return (*this);
  }

  inline Vector & operator[](int q)
  {
   UpdateTransfMatrix();
   return v[q]; 
  }  

  inline const Vector & operator[](int q) const { return v[q]; }

  inline const NonOrthogonalCell & operator*=(double a)
  {
   for (int i=0;i<3;++i) v[i] *= a;
   mustupdate = true;   
   return (*this);
  }

  bool & Periodicity(int i) { return p[i]; }
  const bool & Periodicity(int i) const { return p[i]; }

  Vector ScaleByCell(const Vector & cv) const
  {
   Vector nv;
   for (int j=0;j<3;++j) nv = nv + v[j]*cv[j];
   return nv;
  }

  void ConvertToExternal(Vector & a) const
  {
   if (mustupdate) UpdateTransfMatrix();
   if (nonortg > 1.0E-10)
   {
    double v[3];
    for (int q=0;q<3;++q) v[q] = a[q];
    for (int q=0;q<3;++q)
    {
     a[q] = 0.0;
     for (int p=0;p<3;++p) a[q] += v[p]*itm[p][q];
    }
   }
  }

  void ConvertToExternal(double * a) const
  {
   if (mustupdate) UpdateTransfMatrix();
   if (nonortg > 1.0E-10)
   {
    double v[3];
    for (int q=0;q<3;++q) v[q] = a[q];
    for (int q=0;q<3;++q)
    {
     a[q] = 0.0;
     for (int p=0;p<3;++p) a[q] += v[p]*itm[p][q];
    }
   }
  }

  void ConvertToInternal(Vector & a) const
  {     
   if (mustupdate) UpdateTransfMatrix();
   if (nonortg > 1.0e-10)
   {
    double v[3];
    for (int q=0;q<3;++q) v[q] = a[q];
    for (int q=0;q<3;++q)
    {
     a[q] = 0.0;
     for (int p=0;p<3;++p) a[q] += v[p]*tm[p][q];
    }
   }
  }

  void ConvertToInternal(double * a) const
  {     
   if (mustupdate) UpdateTransfMatrix();
   if (nonortg > 1.0e-10)
   {
    double v[3];
    for (int q=0;q<3;++q) v[q] = a[q];
    for (int q=0;q<3;++q)
    {
     a[q] = 0.0;
     for (int p=0;p<3;++p) a[q] += v[p]*tm[p][q];
    }
   }
  }

  Vector Fractional(const Vector & cv) const
  {
   Vector tmp(cv);
   ConvertToInternal(tmp);
   for (int q=0;q<3;++q) tmp[q] /= v[q].Module();
   return tmp;
  }
  
  Vector Cartesian(const Vector & cv) const
  {
   Vector tmp(cv);
   for (int q=0;q<3;++q) tmp[q] *= v[q].Module();
   ConvertToExternal(tmp);
   return tmp;
  }

  Vector FittedInside(const Vector & a) const
  {
   Vector vtmp(a);
   ConvertToInternal(vtmp);
   for (int q=0;q<3;++q)
   {
    if (p[q] == true)
    {
     double ll = v[q].Module();
     if (vtmp[q] < 0.0) vtmp[q] -= ll*floor(vtmp[q]/ll);
     else if (vtmp[q] > ll) vtmp[q] -= ll*floor(vtmp[q]/ll);
    }
   } 
   ConvertToExternal(vtmp);
   return vtmp;
  }

  bool IsInside(const Vector & a) const
  {
   Vector vtmp(a);
   ConvertToInternal(vtmp);
   for (int q=0;q<3;++q) 
     if ((vtmp[q] < 0.0) || (vtmp[q] > v[q].Module())) return false;
   return true;
  }

  Vector Displacement(const Vector & a, const Vector & b) const;

  inline void FixDisplacement(Vector & d) const
  {
   ConvertToInternal(d);
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
   ConvertToExternal(d);
  }

  inline double FixDisplacement(double * dr) const
  {
   ConvertToInternal(dr);
   double * dd, r2 = 0.0e0;
   for (int i=0;i<3;++i)
   {
    if (p[i] == true)
    {
     dd = dr+i;
     double ll = v[i].Module();
     if (*dd >= 0.5*ll) *dd -= ll;
     else if (*dd < -0.5*ll) *dd += ll;
    }
    r2 += (dr[i]*dr[i]);
   }
   ConvertToExternal(dr);
   return r2;
  }

  inline double Volume() const { return fabs(Dot(Cross(v[0], v[1]), v[2])); }

 private:
   Vector v[3];
   bool p[3];
   mutable double tm[3][3],itm[3][3];
   mutable double nonortg;
   mutable bool mustupdate;

   inline void UpdateTransfMatrix() const
   {
    Vector vnorm[3];
    for (int q=0;q<3;++q)
    {
     vnorm[q] = v[q];
     vnorm[q].Normalize();
    }
    tm[0][0] = vnorm[1][1]*vnorm[2][2]-vnorm[2][1]*vnorm[1][2];
    tm[1][0] = vnorm[2][0]*vnorm[1][2]-vnorm[1][0]*vnorm[2][2];
    tm[2][0] = vnorm[1][0]*vnorm[2][1]-vnorm[2][0]*vnorm[1][1];
    tm[0][1] = vnorm[2][1]*vnorm[0][2]-vnorm[0][1]*vnorm[2][2];
    tm[1][1] = vnorm[0][0]*vnorm[2][2]-vnorm[2][0]*vnorm[0][2];
    tm[2][1] = vnorm[2][0]*vnorm[0][1]-vnorm[0][0]*vnorm[2][1];
    tm[0][2] = vnorm[0][1]*vnorm[1][2]-vnorm[1][1]*vnorm[0][2];
    tm[1][2] = vnorm[1][0]*vnorm[0][2]-vnorm[0][0]*vnorm[1][2];
    tm[2][2] = vnorm[0][0]*vnorm[1][1]-vnorm[1][0]*vnorm[0][1];
    const double d = tm[0][0]*vnorm[0][0] + tm[0][1]*vnorm[1][0] + tm[0][2]*vnorm[2][0];
    nonortg = 0.0;
    double x;
    for (int j=0;j<3;++j)
     for (int i=0;i<3;++i)
     {
      itm[j][i] = vnorm[j][i];
      tm[j][i] = (tm[j][i] / d);
      x = ((i == j) ? 1.0 : 0.0);
      nonortg += ((x-tm[j][i])*(x-tm[j][i]));
     }
    nonortg = sqrt(nonortg);
    mustupdate = false;
   }
};

//
//
inline lpmd::NonOrthogonalCell::NonOrthogonalCell()
{
 v[0]=e1;v[1]=e2;v[2]=e3;
 for (int i=0;i<3;++i) p[i] = true;
 UpdateTransfMatrix();
}

inline lpmd::NonOrthogonalCell::NonOrthogonalCell(const BasicCell & c)
{
 for (int q=0;q<3;++q) 
 { 
  v[q] = c[q];
  p[q] = c.Periodicity(q);
 }
 UpdateTransfMatrix();
}

inline lpmd::NonOrthogonalCell::NonOrthogonalCell(const NonOrthogonalCell & c)
{
 for (int q=0;q<3;++q) 
 { 
  v[q] = c[q];
  p[q] = c.p[q];
 }
 UpdateTransfMatrix();
}

inline lpmd::NonOrthogonalCell::NonOrthogonalCell(const Vector & a, const Vector & b, const Vector & c)
{
 v[0]=a;v[1]=b;v[2]=c;
 for (int i=0;i<3;++i) p[i] = true;
 UpdateTransfMatrix();
}

inline lpmd::NonOrthogonalCell::NonOrthogonalCell(const double a, const double b, const double c, const double alpha, const double beta, const double gamma)
{
 double ar = DegreesToRadians(alpha);
 double br = DegreesToRadians(beta);
 double gr = DegreesToRadians(gamma);
 v[0][0] = a;
 v[0][1] = 0.0e0;
 v[0][2] = 0.0e0;
 v[1][0] = b*cos(gr);
 v[1][1] = b*sin(gr);	          
 v[1][2] = 0.0e0;		   
 v[2][0] = c*cos(br);		   
 double tmp=(cos(ar)-cos(gr)*cos(br))/sin(gr);			  
 v[2][1] = c*tmp;			      
 v[2][2] = c*sqrt(sin(br)*sin(br)-tmp*tmp);		
 for (int i=0;i<3;++i) p[i] = true;
 UpdateTransfMatrix();
}

inline Vector lpmd::NonOrthogonalCell::Displacement(const Vector & a, const Vector & b) const
{
 Vector d = b - a;
 ConvertToInternal(d);
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
 ConvertToExternal(d);
 return d;
}


}  // lpmd

#endif

