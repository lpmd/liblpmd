/*
 *
 *
 *
 */

#ifndef __LPMD_ATOM_H__
#define __LPMD_ATOM_H__

#include <cstdlib>
#include <string.h>
#include "vector.h"

const std::string ElemSym[119]=
{
 "e","H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl",
 "Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br",
 "Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I",
 "Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At",
 "Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Uun","Uuu","Uub","Uut","Uuq","Uup","Uuh","Uus","Uuo"
};

const double ElemMass[119]=
{
 5.4859e-4,1.00794,4.002602,6.941,9.01218,10.811,12.011,14.00674,15.9994,18.9984303,
 20.1797,22.989768,24.305,26.981539,28.0855,30.973762,32.066,35.4527,
 39.948,39.0983,40.078,44.95591,47.88,50.9415,51.9961,54.93805,55.847,58.9332,58.6934,63.546,65.39,69.723,72.61,74.92159,78.96,79.904,
 83.8,85.4678,87.62,88.90585,91.224,92.90638,95.94,97.9072,102.07,102.9055,106.42,107.8682,112.411,114.818,118.71,121.760,127.6,126.90447,
 131.29,132.90543,137.327,138.9055,140.115,140.90765,144.24,144.9127,150.36,151.965,157.25,158.92534,162.50,164.93032,167.26,168.93421,173.04,174.967,178.49,180.9479,183.84,186.207,190.23,192.22,195.08,196.96654,200.59,204.3833,207.2,208.98037,208.9824,209.9871,
 222.0176,223.0197,226.0254,227.0278,232.0381,231.03588,238.0289,237.048,244.0642,243.0614,247.0703,247.0703,251.0796,252.083,257.0951,258.1,259.1009,262.11,261,262,266,264,269,268,269,272,277,0,289,0,0,0,0
};

inline int ElemNum(std::string el)
{
 int r=0;
 for (int i=0;i<119;i++)
 {
  if (el == ElemSym[i])
  {
   r=i;
   break;
  }
 }
 return r;
}


class Atom
{
 public: 
  Atom(): z(0), index(0) { }

  Atom(const std::string a) { z = ElemNum(a); }

  Atom(const int zeta): z(zeta) { }

  Atom(const std::string a, Vector & pos)
  {
   z = ElemNum(a);
   p = pos;
  }
  Atom(const std::string a, Vector & pos, Vector & vel)
  {
   z = ElemNum(a);
   p = pos;
   v = vel;
  }
  Atom(const std::string elem, Vector & pos, Vector & vel, Vector & ace)
  {
   z = ElemNum(elem);
   p = pos;
   v = vel;
   a = ace;
  }

  inline int Z() {return z;}
  inline std::string Symbol() {return ElemSym[z];}
  inline Vector& Position() {return p;}
  inline Vector& Velocity() {return v;}
  inline Vector& Acceleration() {return a;}
  inline double Mass() const {return ElemMass[z];}

 private:
  int z; //atom symbol
  Vector p, v, a;
  long index; //index atom
};

//
//
//


#endif


