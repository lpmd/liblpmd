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
#include "vector3.h"
#include "util.h"

// Se va a util....?
inline double DegreesToRadians(double d) { return (M_PI*d/180.0); }

class Cell
{
 public:
  Cell()
  {
   v[0]=e1;v[1]=e2;v[2]=e3;
   for (int i=0;i<3;++i) p[i] = true;
  }

  Cell(const Vector & a, const Vector & b, const Vector & c)
  {
   v[0]=a;v[1]=b;v[2]=c;
   for (int i=0;i<3;++i) p[i] = true;
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
   //UpdateTransfMatrix();
  }

  Cell(std::string str)
  {
   RemoveUnnecessarySpaces(str);
   std::vector<std::string> vecs = SplitTextLine(str,' ');
   if(vecs.size()==3)
   {
    v[0] = Vector3(vecs[0].c_str());
    v[1] = Vector3(vecs[1].c_str());
    v[2] = Vector3(vecs[2].c_str());
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
  }


  inline Vector & operator[](int q) { return v[q]; }  
  inline const Vector & operator[](int q) const { return v[q]; }

  inline bool & Periodicity(int i) {return p[i];}
 private:
   Vector3 v[3];
   bool p[3];
};

//
//
//

#endif

