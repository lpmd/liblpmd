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
   std::vector<std::string> vecs = StringSplit< std::vector<std::string> >(str,' ');
   if(vecs.size()==3)
   {
    v[0] = Vector(vecs[0].c_str());
    v[1] = Vector(vecs[1].c_str());
    v[2] = Vector(vecs[2].c_str());
   }
   else if(vecs.size()==6)
   {
    Cell tmp(atof(vecs[0].c_str()),atof(vecs[1].c_str()),atof(vecs[2].c_str()),atof(vecs[3].c_str()),atof(vecs[4].c_str()),atof(vecs[5].c_str()));
    (*this) = tmp;
   }
   else if(vecs.size()==9)
   {
    v[0][0] = atof(vecs[0].c_str());
    v[0][1] = atof(vecs[1].c_str());
    v[0][2] = atof(vecs[2].c_str());
    v[1][0] = atof(vecs[3].c_str());
    v[1][1] = atof(vecs[4].c_str());
    v[1][2] = atof(vecs[5].c_str());
    v[2][0] = atof(vecs[6].c_str());
    v[2][1] = atof(vecs[7].c_str());
    v[2][2] = atof(vecs[8].c_str());
   }
   else
   {
    //FIXME : Error!
   }
   UpdateTransfMatrix();
  }


  inline Vector & operator[](int q) { return v[q]; }  
  inline const Vector & operator[](int q) const { return v[q]; }
  inline const Cell & operator*=(double a)
  {
   for (int i=0;i<3;++i) v[i] *= a;
   return (*this);
  }

  inline bool & Periodicity(int i) {return p[i];}
  inline Vector ScaleByCell(const Vector & cv) const
  {
   Vector nv;
   for (int j=0;j<3;++j) nv = nv + v[j]*cv[j];
   return nv;
  }

  inline void UpdateTransfMatrix()
  {
   Vector vnorm[3];
   for (int i=0;i<3;++i)
   {
    vnorm[i] = v[i];
    vnorm[i].Normalize();
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
  }
  inline void ConvertToExternal(Vector & a)
  {
   if (nonortg > 1.0E-10)
   {
    const double x = a[0];
    const double y = a[1];
    const double z = a[2];
    a[0] = x*itm[0][0]+y*itm[1][0]+z*itm[2][0];
    a[1] = x*itm[0][1]+y*itm[1][1]+z*itm[2][1];
    a[2] = x*itm[0][2]+y*itm[1][2]+z*itm[2][2];
   }
  }
  inline void ConvertToInternal(Vector & a)
  {     
   if (nonortg > 1.0e-10)
   {
    const double x = a[0];
    const double y = a[1];
    const double z = a[2];
    a[0] = x*tm[0][0]+y*tm[1][0]+z*tm[2][0];
    a[1] = x*tm[0][1]+y*tm[1][1]+z*tm[2][1];
    a[2] = x*tm[0][2]+y*tm[1][2]+z*tm[2][2];
   }
  }

 private:
   Vector v[3];
   bool p[3];
   double tm[3][3],itm[3][3];
   double nonortg;
};

}

#endif

