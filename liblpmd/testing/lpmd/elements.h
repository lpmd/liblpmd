/*
 *
 *
 *
 */

#ifndef __LPMD_ELEMENTS_H__
#define __LPMD_ELEMENTS_H__

#include <string>

extern std::string ElemSym[119];

extern double ElemMass[119];

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

#endif

