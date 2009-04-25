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
#include "vector.h"
#include "util.h"

class Cell
{
 public:
  Cell()
  {
   v[0]=e1;v[1]=e2;v[2]=e3;
   for (int i=0;i<3;++i) p[i] = true;
  }
  Cell(const Vector a, const Vector b, const Vector c)
  {
   v[0]=a;v[1]=b;v[2]=c;
   for (int i=0;i<3;++i) p[i] = true;
  }
  Cell(const double a, const double b, const double c, const double alpha, const double beta, const double gamma)
  {
   v[0][0] = a;
   v[0][1] = 0.0e0;
   v[0][2] = 0.0e0;
   v[1][0] = b*cos(gamma);
   v[1][1] = b*sin(gamma);	          
   v[1][2] = 0.0e0;		   
   v[2][0] = c*cos(beta);		   
   double tmp=(cos(alpha)-cos(gamma)*cos(beta))/sin(gamma);			  
   v[2][1] = c*tmp;			      
   v[2][3] = c*sqrt(sin(beta)*sin(beta)-tmp*tmp);		
   for (int i=0;i<3;++i) p[i] = true;
   //UpdateTransfMatrix();
  }
  Cell(std::string str)
  {
   RemoveUnnecessarySpaces(str);
   std::vector<std::string> vecs = SplitTextLine(str,' ');
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
  }


  inline Vector & operator[](int q) { return v[q]; }  
  inline const Vector & operator[](int q) const { return v[q]; }

  inline bool & Periodicity(int i) {return p[i];}
 private:
   Vector v[3];
   bool p[3];
};

//
//
//

#endif

