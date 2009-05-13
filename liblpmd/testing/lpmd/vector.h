/*
 *
 *
 *
 */

#ifndef __LPMD_VECTOR_H__
#define __LPMD_VECTOR_H__

#include <cmath>
#include <iostream>
#include <cctype>
#include <string.h>
#include <cstdlib>
#include <cassert>

namespace lpmd
{

class Vector
{
 public:

   Vector() 
   { 
    for (int q=0;q<3;++q) v[q] = 0.0;
   }

   Vector(const Vector & a)
   {
    for (int q=0;q<3;++q) v[q] = a.v[q];
   }

   Vector & operator=(const Vector & a)
   {
    if (this != &a) 
    {
     for (int q=0;q<3;++q) v[q] = a.v[q];
    }
    return (*this);
   }

   Vector(double x, double y, double z)
   {
    v[0] = x; v[1] = y; v[2] = z;
   }
 
   Vector(double * w) 
   {
    for (int q=0;q<3;++q) v[q] = w[q];
   }  

   Vector(const char * str)
   {
    char * s = (char *)(malloc((strlen(str)+1)*sizeof(char)));
    strncpy(s, str, strlen(str));
    char * p = s, * start = NULL;
    int k = 0;
    while (isblank(*p)) p++;
    if (*p == '<')
    {
     start = ++p;
     while (*p != '\0')
     {
      if ((*p == ',') || (*p == '>'))
      {
       if (k == 3) { } // FIXME: raise error, too many commas
       (*p) = '\0';
       v[k++] = atof(start);
       p++;
       start = p;
      }
      else p++;
     }
     assert(k == 3);  // change assert to an exception
    }
    else
    {
     start = p;
     while (1)
     {
      if (isblank(*p) || (*p == '\0'))
      {
       if (k == 3) { } // FIXME: raise error, too many delimiters
       (*p) = '\0';
       v[k++] = atof(start);
       p++;
       start = p;
       if (k == 3) break;
      }
      else p++;
     }
    }
    free(s);
   }

   inline double & operator[](int q) { return v[q]; }

   inline const double & operator[](int q) const { return v[q]; }

   inline void Normalize() 
   { 
    double mod = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); 
    for (int q=0;q<3;++q) v[q] /= mod;
   }

   inline double Module() const { return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); }

   inline double SquareModule() const { return v[0]*v[0]+v[1]*v[1]+v[2]*v[2]; }

   inline double Azimuth() const { return atan2(v[1], v[0]); }

   inline double Zenith() const { return acos(v[2]/Module()); }

   inline Vector & operator*=(const double a)
   {
    for (int q=0;q<3;++q) v[q] *= a;
    return (*this);
   }
   
   inline Vector & operator+=(const Vector & a)
   {
    for (int q=0;q<3;++q) v[q] += a[q];
    return (*this);
   }

   inline Vector & operator-=(const Vector & a)
   {
    for (int q=0;q<3;++q) v[q] -= a[q];
    return (*this);
   }

 private:
   double v[3];
};

//
//
//

inline double Dot(const Vector & a, const Vector & b)
{
 return (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]);
}

inline bool operator==(const Vector & a, const Vector & b)
{
 for (int q=0;q<3;++q) 
     if (fabs(a[q]-b[q]) >= 1.0E-10) return false;
 return true;
}

inline std::ostream & operator<<(std::ostream & os, const Vector & v)
{
 os << v[0] << " " << v[1] << " " << v[2];
 return os;
}

inline void FormattedWrite(std::ostream & os, const Vector & v)
{
 os << "< " << v[0] << ", " << v[1] << ", " << v[2] << " >";
}

const Vector e1(1.0, 0.0, 0.0);
const Vector e2(0.0, 1.0, 0.0);
const Vector e3(0.0, 0.0, 1.0);

inline Vector RandomVector(double m=1.0)
{
 double w[3];
 for (int q=0;q<3;++q) w[q] = 2.0*drand48()-1.0;
 double r = Vector(w).Module();
 return Vector(m*w[0]/r, m*w[1]/r, m*w[2]/r);
}

inline Vector operator+(const Vector & a, const Vector & b)
{
 return Vector(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

inline Vector operator-(const Vector & a, const Vector & b)
{
 return Vector(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

inline Vector operator*(const Vector & a, const double b)
{
 return Vector(a[0]*b, a[1]*b, a[2]*b);
}

inline Vector operator*(const double & b, const Vector & a)
{
 return Vector(a[0]*b, a[1]*b, a[2]*b);
}

inline Vector operator/(const Vector & a, const double b)
{
 return Vector(a[0]/b, a[1]/b, a[2]/b);
}

inline Vector Cross(const Vector & a, const Vector & b)
{
 return Vector(a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0]);
}

}

#endif

