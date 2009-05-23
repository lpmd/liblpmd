/*
 *
 *
 *
 */

#ifndef __LPMD_REFPARTICLESET_H__
#define __LPMD_REFPARTICLESET_H__

#include <lpmd/basicparticleset.h>

namespace lpmd
{

 class RefParticleSet: public BasicParticleSet, private Array<BasicAtom &>
 {
  public: 
    inline BasicAtom & operator[](long int i) { return Array<BasicAtom &>::operator[](i); }
 
    inline const BasicAtom & operator[](long int i) const { return Array<BasicAtom &>::operator[](i); }
 
    inline long int Size() const { return Array<BasicAtom &>::Size(); }
 
    inline long int Find(const BasicAtom & t) { return Array<BasicAtom &>::Find(t); }

    inline void Append(const BasicAtom & x) { Array<BasicAtom &>::Append(x); }

    inline void Clear() { Array<BasicAtom &>::Clear(); }

    inline void Delete(long int i) { return Array<BasicAtom &>::Delete(i); }
    
    inline const Array<int> & Elements() const
    {
     // FIXME: esta operacion va a ser costosa si se llama a cada rato
     elements.Clear();
     for (long int i=0;i<Size();++i) elements.AppendUnique((*this)[i].Z());
     return elements; 
    }
     
    inline const Array<int> WithZ(int z) const
    {
     // FIXME: esta operacion va a ser costosa si se llama a cada rato
     Array<int> elwz;
     for (long int i=0;i<Size();++i)
     {
      if ((*this)[i].Z() == z ) {elwz.Append(i);}
     }
     return elwz;
    }

    inline Vector CenterOfMass() const
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

  private:
    mutable Array<int> elements;
 };

}  // lpmd

#endif


