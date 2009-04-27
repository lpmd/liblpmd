/*
 *
 *
 *
 */

#ifndef __LPMD_VECTOR_H__
#define __LPMD_VECTOR_H__

#include <cmath>

// Esta es la interfaz de Vector
// debe ser implementada por las clases concretas

class Vector
{
 public:

   Vector(): inner(0) { }
   virtual ~Vector() { };

   virtual Vector & operator=(const Vector & v) = 0;

   inline double & operator[](int q) { return inner[q]; }
   inline const double & operator[](int q) const { return inner[q]; }
   inline void Normalize() 
   { 
    double mod = sqrt(inner[0]*inner[0]+inner[1]*inner[1]+inner[2]*inner[2]); 
    for(int i=0;i< 3;++i) inner[i]=inner[i]/mod; 
   }
   inline double Module() const { return sqrt(inner[0]*inner[0]+inner[1]*inner[1]+inner[2]*inner[2]); }
   inline double SquareModule() const { return inner[0]*inner[0]+inner[1]*inner[1]+inner[2]*inner[2]; }
   inline double Azimuth() const { return atan2(inner[1], inner[0]); }
   inline double Zenith() const { return acos(inner[2]/Module()); }

 protected:
   double * inner;
};

//
//
//

inline double Dot(const Vector & a, const Vector & b)
{
 return (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]);
}

#endif

