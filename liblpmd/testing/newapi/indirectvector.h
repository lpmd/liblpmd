/*
 *
 *
 *
 */

#ifndef __LPMD_INDIRECT_VECTOR_H__
#define __LPMD_INDIRECT_VECTOR_H__

#include <lpmd/basicvector.h>

namespace lpmd
{

class IndirectVector: public BasicVector
{
 public:
   IndirectVector() {   };
   ~IndirectVector() {   };
  
   IndirectVector & operator=(const BasicVector & v)
   {
    if (this != &v) 
    {
     for (int q=0;q<3;++q) inner[q] = v[q];
    }
    return (*this);
   }

   inline void SetAddress(double * p) { inner = p; }
};

}

#endif

