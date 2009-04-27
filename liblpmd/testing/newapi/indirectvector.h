/*
 *
 *
 *
 */

#ifndef __LPMD_INDIRECT_VECTOR_H__
#define __LPMD_INDIRECT_VECTOR_H__

#include "vector.h"

class IndirectVector: public Vector
{
 public:
   IndirectVector() {   };
   ~IndirectVector() {   };
  
   IndirectVector & operator=(const Vector & v)
   {
    if (this != &v) 
    {
     for (int q=0;q<3;++q) inner[q] = v[q];
    }
    return (*this);
   }

   inline void SetAddress(double * p) { inner = p; }
};

#endif

