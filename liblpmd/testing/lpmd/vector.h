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
#include <lpmd/error.h>

namespace lpmd
{

template <typename T> class GenericVector
{
 public:

   GenericVector() 
   { 
    for (int q=0;q<3;++q) v[q] = T();
   }

   GenericVector(const GenericVector<T> & a)
   {
    for (int q=0;q<3;++q) v[q] = a.v[q];
   }

   GenericVector<T> & operator=(const GenericVector<T> & a)
   {
    if (this != &a) 
    {
     for (int q=0;q<3;++q) v[q] = a.v[q];
    }
    return (*this);
   }

   GenericVector(const T & x, const T & y, const T & z)
   {
    v[0] = x; v[1] = y; v[2] = z;
   }
 
   GenericVector(T * w) 
   {
    for (int q=0;q<3;++q) v[q] = w[q];
   }  

   GenericVector(const char * str)
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
       char token = (*p);
       (*p) = '\0';
       if (k == 3) throw SyntaxError("Too many commas, GenericVector("+std::string(str)+")");
       v[k++] = atof(start);
       if (token == '>') break;
       p++;
       start = p;
      }
      else p++;
     }
     if (k != 3) throw SyntaxError("GenericVector("+std::string(str)+")");
    }
    else
    {
     start = p;
     while (1)
     {
      if (isblank(*p) || (*p == '\0'))
      {
       if (k == 3) throw SyntaxError("Too many delimiters, GenericVector("+std::string(str)+")");
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

   inline T & operator[](int q) { return v[q]; }

   inline const T & operator[](int q) const { return v[q]; }

   inline void Normalize() 
   { 
    const T mod = Module();
    for (int q=0;q<3;++q) v[q] /= mod;
   }

   inline T Module() const { return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); }

   inline T SquareModule() const { return v[0]*v[0]+v[1]*v[1]+v[2]*v[2]; }

   inline T Azimuth() const { return atan2(v[1], v[0]); }

   inline T Zenith() const { return acos(v[2]/Module()); }

   inline GenericVector<T> & operator*=(const T a)
   {
    for (int q=0;q<3;++q) v[q] *= a;
    return (*this);
   }
   
   inline GenericVector<T> & operator+=(const GenericVector<T> & a)
   {
    for (int q=0;q<3;++q) v[q] += a[q];
    return (*this);
   }

   inline GenericVector<T> & operator-=(const GenericVector<T> & a)
   {
    for (int q=0;q<3;++q) v[q] -= a[q];
    return (*this);
   }

 private:
   T v[3];
};

typedef GenericVector<double> Vector;

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

inline void FormattedWrite(std::ostream & os, const Vector & v)
{
 os << "< " << v[0] << ", " << v[1] << ", " << v[2] << " >";
}

inline std::ostream & operator<<(std::ostream & os, const Vector & v)
{
 FormattedWrite(os, v);
 // os << v[0] << " " << v[1] << " " << v[2];
 return os;
}

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

//
//
//

const Vector e1(1.0, 0.0, 0.0);
const Vector e2(0.0, 1.0, 0.0);
const Vector e3(0.0, 0.0, 1.0);

const Vector identity[3] = { e1, e2, e3 };

}

#endif

