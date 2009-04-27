/*
 *
 *
 *
 */

#ifndef __LPMD_VECTOR3_H__
#define __LPMD_VECTOR3_H__

#include "vector.h"
#include <cctype>
#include <string.h>
#include <cstdlib>
#include <cassert>

class Vector3: public Vector
{
 public:
    Vector3()
    { 
     inner = new double[3]; 
     for (int q=0;q<3;++q) inner[q] = 0.0;
    }

    Vector3(const Vector3 & v) 
    {
     inner = new double[3]; 
     for (int q=0;q<3;++q) inner[q] = v[q];
    }
   
    Vector3(double x, double y, double z)
    {
     inner = new double[3]; 
     inner[0] = x; inner[1] = y; inner[2] = z;
    }
 
    Vector3(double * w) 
    {
     inner = new double[3]; 
     for (int q=0;q<3;++q) inner[q] = w[q];
    }  

    Vector3(const char * str)
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

    ~Vector3() { delete [] inner; }

   Vector3 & operator=(const Vector3 & v)
   {
    if (this != &v) 
    {
     for (int q=0;q<3;++q) inner[q] = v[q];
    }
    return (*this);
   }

   Vector3 & operator=(const Vector & v)
   {
    if (this != &v) 
    {
     for (int q=0;q<3;++q) inner[q] = v[q];
    }
    return (*this);
   }

};

const Vector3 e1(1.0, 0.0, 0.0);
const Vector3 e2(0.0, 1.0, 0.0);
const Vector3 e3(0.0, 0.0, 1.0);

inline Vector3 RandomVector(double m=1.0)
{
 double w[3];
 for (int q=0;q<3;++q) w[q] = 2.0*drand48()-1.0;
 double r = Vector3(w).Module();
 return Vector3(m*w[0]/r, m*w[1]/r, m*w[2]/r);
}

inline Vector3 operator+(const Vector & a, const Vector & b)
{
 return Vector3(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

inline Vector3 operator-(const Vector & a, const Vector & b)
{
 return Vector3(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

inline Vector3 operator*(const Vector & a, const double b)
{
 return Vector3(a[0]*b, a[1]*b, a[2]*b);
}

inline Vector3 operator*(const double & b, const Vector & a)
{
 return Vector3(a[0]*b, a[1]*b, a[2]*b);
}

inline Vector3 operator/(const Vector & a, const double b)
{
 return Vector3(a[0]/b, a[1]/b, a[2]/b);
}

inline Vector3 Cross(const Vector & a, const Vector & b)
{
 return Vector3(a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0]);
}

#endif

