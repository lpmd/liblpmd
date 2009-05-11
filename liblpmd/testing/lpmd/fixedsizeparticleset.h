/*
 *
 *
 *
 */

#ifndef __LPMD_FIXEDSIZEPARTICLESET_H__
#define __LPMD_FIXEDSIZEPARTICLESET_H__

#include <lpmd/indirectvector.h>
#include <lpmd/indirectatom.h>
#include <lpmd/basicparticleset.h>

namespace lpmd
{

class FixedSizeParticleSet: public AtomArray
{
 public: 
   FixedSizeParticleSet(long int n): nl(n)
   {
    atomarray = new IndirectAtom[n];
    vectorarray = new IndirectVector[3*n];
    storage = new double[9*n];
    for (long int i=0;i<3*n;++i) vectorarray[i].SetAddress(storage+3*i);
    for (long int i=0;i<n;++i) atomarray[i].SetAddress(vectorarray+3*i);
   }

   virtual ~FixedSizeParticleSet() 
   { 
    delete [] storage;
    delete [] vectorarray;
    delete [] atomarray; 
   }

   inline AtomInterface & operator[](long int i) { return atomarray[i]; }
   inline const AtomInterface & operator[](long int i) const { return atomarray[i]; }

   inline long int Size() const { return nl; }

   inline long int Find(const AtomInterface & t)
   {
    for (long int i=0;i<Size();++i) 
    {
     if ((*this)[i] == t) return i;
    }
    return -1;
   }

 private:
   long int nl;
   IndirectAtom * atomarray;
   IndirectVector * vectorarray;
   double * storage;
};

}  // lpmd

#endif


