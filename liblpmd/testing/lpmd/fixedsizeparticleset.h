/*
 *
 *
 *
 */

#ifndef __LPMD_FIXEDSIZEPARTICLESET_H__
#define __LPMD_FIXEDSIZEPARTICLESET_H__

#include <lpmd/indirectatom.h>
#include <lpmd/basicparticleset.h>

namespace lpmd
{

class FixedSizeParticleSet: public AtomArray
{
 public: 
   FixedSizeParticleSet(long int n): nl(n)
   {
    AllocateMemory(n);
   }

   FixedSizeParticleSet(long int n, const AtomInterface & at): nl(n)
   {
    AllocateMemory(n);
    for (long int i=0;i<n;++i) (*this)[i] = at;
   }

   void AllocateMemory(long int n)
   {
    storepos = new double[3*n];
    storevel = new double[3*n];
    storeacc = new double[3*n];
    posarray = new(storepos) Vector[n];
    velarray = new(storevel) Vector[n];
    accarray = new(storeacc) Vector[n];
    atomarray = new IndirectAtom[n];
    for (long int i=0;i<n;++i) 
        atomarray[i].SetAddresses(&posarray[i], &velarray[i], &accarray[i]);
   }

   virtual ~FixedSizeParticleSet() 
   { 
    delete [] atomarray;
    delete [] storepos;
    delete [] storevel;
    delete [] storeacc;
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
   double * storepos, * storevel, * storeacc;
   Vector * posarray, * velarray, * accarray;
};

}  // lpmd

#endif

