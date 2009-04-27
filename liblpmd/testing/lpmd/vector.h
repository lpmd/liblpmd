/*
 *
 *
 *
 */

#ifndef __LPMD_VECTOR_H__
#define __LPMD_VECTOR_H__

#include "basicvector.h"
#include <cctype>
#include <string.h>
#include <cstdlib>
#include <cassert>

namespace lpmd
{

class Vector: public BasicVector
{
 public:
    Vector()
    { 
     inner = new double[3]; 
     for (int q=0;q<3;++q) inner[q] = 0.0;
    }

    Vector(const Vector & v) 
    {
     inner = new double[3]; 
     for (int q=0;q<3;++q) inner[q] = v[q];
    }
   
    Vector(double x, double y, double z)
    {
     inner = new double[3]; 
     inner[0] = x; inner[1] = y; inner[2] = z;
    }
 
    Vector(double * w) 
    {
     inner = new double[3]; 
     for (int q=0;q<3;++q) inner[q] = w[q];
    }  

    Vector(const char * str)
    {
     inner = new double[3]; 
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
        inner[k++] = atof(start);
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
        inner[k++] = atof(start);
        p++;
        start = p;
        if (k == 3) break;
       }
       else p++;
      }
     }
     free(s);
    }

    ~Vector() { delete [] inner; }

   Vector & operator=(const Vector & v)
   {
    if (this != &v) 
    {
     for (int q=0;q<3;++q) inner[q] = v[q];
    }
    return (*this);
   }

   Vector & operator=(const BasicVector & v)
   {
    if (this != &v) 
    {
     for (int q=0;q<3;++q) inner[q] = v[q];
    }
    return (*this);
   }

};

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

inline Vector operator+(const BasicVector & a, const BasicVector & b)
{
 return Vector(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

inline Vector operator-(const BasicVector & a, const BasicVector & b)
{
 return Vector(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

inline Vector operator*(const BasicVector & a, const double b)
{
 return Vector(a[0]*b, a[1]*b, a[2]*b);
}

inline Vector operator*(const double & b, const BasicVector & a)
{
 return Vector(a[0]*b, a[1]*b, a[2]*b);
}

inline Vector operator/(const BasicVector & a, const double b)
{
 return Vector(a[0]/b, a[1]/b, a[2]/b);
}

inline Vector Cross(const BasicVector & a, const BasicVector & b)
{
 return Vector(a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0]);
}

}

#endif

