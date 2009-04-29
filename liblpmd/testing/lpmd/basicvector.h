/*
 *
 *
 *
 */

#ifndef __LPMD_BASICVECTOR_H__
#define __LPMD_BASICVECTOR_H__

#include <cmath>
#include <iostream>

// Esta es la interfaz de Vector
// debe ser implementada por las clases concretas
namespace lpmd
{

class BasicVector
{
 public:

   BasicVector(): inner(0) { }
   virtual ~BasicVector() { };

   virtual BasicVector & operator=(const BasicVector & v) = 0;

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
   inline BasicVector & operator*=(const double a)
   {
    for (int j=0;j<3;++j) inner[j] *= a;
    return (*this);
   }
 protected:
   double * inner;
};

//
//
//

inline double Dot(const BasicVector & a, const BasicVector & b)
{
 return (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]);
}

inline std::ostream & operator<<(std::ostream & os, const BasicVector & v)
{
 os << v[0] << " " << v[1] << " " << v[2];
 return os;
}

inline void FormattedWrite(std::ostream & os, const BasicVector & v)
{
 os << "< " << v[0] << ", " << v[1] << ", " << v[2] << " >";
}


}

#endif

