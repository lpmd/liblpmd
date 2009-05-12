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

   AtomInterface & operator[](long int i) { return Array<Atom>::operator[](i); }

   const AtomInterface & operator[](long int i) const { return Array<Atom>::operator[](i); }

   long int Size() const { return Array<Atom>::Size(); }

   long int Find(const AtomInterface & at) { return Array<Atom>::Find(at); }

   void Append(const AtomInterface & at) { Array<Atom>::Append(at); }

   void Clear() { Array<Atom>::Clear(); }

   void Delete(long int i) { Array<Atom>::Delete(i); } 
 };

} // lpmd

#endif

