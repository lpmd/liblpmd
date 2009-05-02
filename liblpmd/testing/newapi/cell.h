/*
 *
 *
 *
 */

#ifndef __LPMD_CELL_H__
#define __LPMD_CELL_H__

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <string.h>
#include <lpmd/vector.h>
#include <lpmd/util.h>

namespace lpmd
{

class Cell
{

 public:
  Cell()
  {
   v[0]=e1;v[1]=e2;v[2]=e3;
   for (int i=0;i<3;++i) p[i] = true;
   UpdateTransfMatrix();
  }

  Cell(const Vector & a, const Vector & b, const Vector & c)
  {
   v[0]=a;v[1]=b;v[2]=c;
   for (int i=0;i<3;++i) p[i] = true;
   UpdateTransfMatrix();
  }

  Cell(const double a, const double b, const double c, const double alpha, const double beta, const double gamma)
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

  Cell(std::string str)
  {
   RemoveUnnecessarySpaces(str);
   if (str[0] == '<') 
   {
    std::string bv[3];
    int start = 0, k=0;
    for (unsigned int i=0;i<str.size();++i)
    {
     if (str[i] == '>') 
     {
      bv[k++] = str.substr(start, i-start+1);
      start = -1;
     } 
     if ((start == -1) && (str[i] == '<'))
     {
      start = i;
     }
    }
    (*this) = Cell(Vector(bv[0].c_str()), Vector(bv[1].c_str()), Vector(bv[2].c_str()));
   }
   else
   {
    std::vector<std::string> vecs = StringSplit< std::vector<std::string> >(str,' ');
    if (vecs.size() == 3)
    {
     for (int q=0;q<3;++q) v[q] = Vector(vecs[q].c_str());
    }
    else if(vecs.size()==6)
    {
     Cell tmp(atof(vecs[0].c_str()),atof(vecs[1].c_str()),atof(vecs[2].c_str()),atof(vecs[3].c_str()),atof(vecs[4].c_str()),atof(vecs[5].c_str()));
     (*this) = tmp;
    }
    else if(vecs.size()==9)
    {
     for (int p=0;p<3;++p)
       for (int q=0;q<3;++q) v[p][q] = atof(vecs[q+3*p].c_str());
    }
    else
    {
     //FIXME : Error!
    }
    UpdateTransfMatrix();
   }
  }

  inline Cell & operator=(const Cell & c)
  {
   if (this != &c)
   {
    for (int q=0;q<3;++q) v[q] = c[q];
    UpdateTransfMatrix();
   }
   return (*this);
  }

  inline Vector & operator[](int q)
  { 
   mustupdate = true;   
   return v[q]; 
  }  

  inline const Vector & operator[](int q) const { return v[q]; }

  inline const Cell & operator*=(double a)
  {
   for (int i=0;i<3;++i) v[i] *= a;
   mustupdate = true;   
   return (*this);
  }

  inline bool & Periodicity(int i) { return p[i]; }
  inline const bool & Periodicity(int i) const { return p[i]; }

  inline Vector ScaleByCell(const Vector & cv) const
  {
   Vector nv;
   for (int j=0;j<3;++j) nv = nv + v[j]*cv[j];
   return nv;
  }

  inline void UpdateTransfMatrix()
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

  inline void ConvertToExternal(Vector & a)
  {
   if (nonortg > 1.0E-10)
   {
    if (mustupdate) UpdateTransfMatrix();
    double v[3];
    for (int q=0;q<3;++q) v[q] = a[q];
    for (int q=0;q<3;++q)
    {
     a[q] = 0.0;
     for (int p=0;p<3;++p) a[q] += v[p]*itm[p][q];
    }
   }
  }

  inline void ConvertToInternal(Vector & a)
  {     
   if (nonortg > 1.0e-10)
   {
    if (mustupdate) UpdateTransfMatrix();
    double v[3];
    for (int q=0;q<3;++q) v[q] = a[q];
    for (int q=0;q<3;++q)
    {
     a[q] = 0.0;
     for (int p=0;p<3;++p) a[q] += v[p]*tm[p][q];
    }
   }
  }

 private:
   Vector v[3];
   bool p[3];
   double tm[3][3],itm[3][3];
   double nonortg;
   bool mustupdate;
};

}

#endif

