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

   ParticleSet(long int s, const BasicAtom & at): Array<Atom>(s, at) { }

   ParticleSet(const AtomArray & arr): Array<Atom>(arr.Size())
   {
    for (long int i=0;i<arr.Size();++i) (*this)[i] = arr[i];
   }

   BasicAtom & operator[](long int i) { return Array<Atom>::operator[](i); }

   const BasicAtom & operator[](long int i) const { return Array<Atom>::operator[](i); }

   long int Size() const { return Array<Atom>::Size(); }

   long int Find(const BasicAtom & at) { return Array<Atom>::Find(at); }

   void Append(const BasicAtom & at) { Array<Atom>::Append(at); }

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

   void Clear() { Array<Atom>::Clear(); }

   void Delete(long int i) { Array<Atom>::Delete(i); } 

 private:
   mutable Array<int> elements;
 };

} // lpmd

#endif

