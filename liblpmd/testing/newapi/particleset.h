/*
 *
 *
 *
 */

#ifndef __LPMD_PARTICLESET_H__
#define __LPMD_PARTICLESET_H__

#include <lpmd/atominterface.h>
#include "indirectvector.h"
#include "indirectatom.h"

namespace lpmd
{

class ParticleSet
{
 public: 
   ParticleSet(): nl(0), atomarray(0), vectorarray(0), storage(0) { }
 
   ParticleSet(long int n): nl(n)
   {
    atomarray = new IndirectAtom[n];
    vectorarray = new IndirectVector[3*n];
    storage = new double[9*n];
    for (long int i=0;i<3*n;++i) vectorarray[i].SetAddress(storage+3*i);
    for (long int i=0;i<n;++i) atomarray[i].SetAddress(vectorarray+3*i);
   }

   virtual ~ParticleSet() 
   { 
    delete [] storage;
    delete [] vectorarray;
    delete [] atomarray; 
   }

   inline AtomInterface & operator[](long int i) { return atomarray[i]; }
   inline long int Size() const { return nl; }

 private:
   long int nl;
   IndirectAtom * atomarray;
   IndirectVector * vectorarray;
   double * storage;
};

}

#endif


