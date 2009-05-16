//
//
//

#ifndef __LPMD_PARTICLESET_H__
#define __LPMD_PARTICLESET_H__

#include <lpmd/atom.h>
#include <lpmd/array.h>
#include <lpmd/basicparticleset.h>

namespace lpmd
{

 class ParticleSet: public Array<Atom>, public BasicParticleSet
 {
  public:
   ParticleSet(): Array<Atom>() { }

   ParticleSet(long int s): Array<Atom>(s) { }

   ParticleSet(long int s, const AtomInterface & at): Array<Atom>(s, at) { }

   ParticleSet(const AtomArray & arr): Array<Atom>(arr.Size())
   {
    for (long int i=0;i<arr.Size();++i) (*this)[i] = arr[i];
   }

   AtomInterface & operator[](long int i) { return Array<Atom>::operator[](i); }

   const AtomInterface & operator[](long int i) const { return Array<Atom>::operator[](i); }

   long int Size() const { return Array<Atom>::Size(); }

   long int Find(const AtomInterface & at) { return Array<Atom>::Find(at); }

   void Append(const AtomInterface & at) { Array<Atom>::Append(at); }

   const Array<int> & Elements() const
   {
    // FIXME: esta operacion va a ser costosa si se llama a cada rato
    elements.Clear();
    for (long int i=0;i<Size();++i) elements.AppendUnique((*this)[i].Z());
    return elements; 
   }

   void Clear() { Array<Atom>::Clear(); }

   void Delete(long int i) { Array<Atom>::Delete(i); } 

 private:
   mutable Array<int> elements;
 };

} // lpmd

#endif

