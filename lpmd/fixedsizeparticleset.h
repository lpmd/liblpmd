/*
 *
 *
 *
 */

#ifndef __LPMD_FIXEDSIZEPARTICLESET_H__
#define __LPMD_FIXEDSIZEPARTICLESET_H__

#include <lpmd/indirectatom.h>
#include <lpmd/basicparticleset.h>
#include <lpmd/error.h>

namespace lpmd
{

 class FixedSizeParticleSet: public BasicParticleSet
 {
  public: 
    FixedSizeParticleSet(long int n): nl(n), iter(0) 
    { 
     AllocateMemory(n); 
     for (long int i=0;i<n;++i) (*this)[i].RenewID();
    }

    FixedSizeParticleSet(long int n, const BasicAtom & at): nl(n), iter(0)
    {
     AllocateMemory(n);
     for (long int i=0;i<n;++i)
     {
      (*this)[i] = at;
      (*this)[i].RenewID();
     }
     iter = nl;
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

    inline BasicAtom & operator[](long int i) { return atomarray[i]; }
 
    inline const BasicAtom & operator[](long int i) const { return atomarray[i]; }
 
    inline long int Size() const { return iter; }
 
    inline long int Find(const BasicAtom & t)
    {
     for (long int i=0;i<Size();++i) 
     {
      if ((*this)[i] == t) return i;
     }
     return -1;
    }

    const Array<int> & Elements() const
    {
     // FIXME: esta operacion va a ser costosa si se llama a cada rato
     elements.Clear();
     for (long int i=0;i<Size();++i) elements.AppendUnique((*this)[i].Z());
     return elements; 
    }
 
   const Array<int> WithZ(int z) const
   {
     // FIXME: esta operacion va a ser costosa si se llama a cada rato
    Array<int> elwz;
    for (long int i=0;i<Size();++i)
    {
     if ((*this)[i].Z() == z ) {elwz.Append(i);}
    }
    return elwz;
   }

   Vector CenterOfMass() const
   {
    Vector cm; 
    double mass = 0.0;
    for (long int i=0;i<Size();++i) 
    {
     mass += (*this)[i].Mass();
     cm += (*this)[i].Mass()*(*this)[i].Position();
    }
    return cm/mass;
   }

   void Append(const BasicAtom & x) 
   { 
    if (iter < nl) (*this)[iter++] = x;
    else throw InvalidOperation("Appending over the maximum size of a FixedSizeParticleSet");
   }

   void Clear() { iter = 0; }

   void Delete(long int i) 
   { 
    for (long int j=i+1;j<nl+1;++j) (*this)[j-1] = (*this)[j];
    nl--;
    iter = nl;
   }

 private:
   long int nl, iter;
   IndirectAtom * atomarray;
   double * storepos, * storevel, * storeacc;
   Vector * posarray, * velarray, * accarray;
   mutable Array<int> elements;
 };

}  // lpmd

#endif


