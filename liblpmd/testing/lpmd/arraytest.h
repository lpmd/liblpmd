/*
 *
 *
 *
 */

#ifndef __LPMD_ARRAY_TEST_H__
#define __LPMD_ARRAY_TEST_H__

#include <lpmd/basicparticleset.h>

namespace lpmd
{

class SlowArray: public BasicParticleSet, private Array<Atom>
{
 public:

  inline AtomInterface & operator[](long int i) { return Array<Atom>::operator[](i); }

  inline const AtomInterface & operator[](long int i) const { return Array<Atom>::operator[](i); }

  inline long int Size() const { return Array<Atom>::Size(); }

  inline void Append(const AtomInterface & x) { Array<Atom>::Append(x); }

  inline void Clear() { Array<Atom>::Clear(); }

  inline void Delete(long int i) { Array<Atom>::Delete(i); }

  inline long int Find(const AtomInterface & t) { return Array<Atom>::Find(t); }
};

} // lpmd

#endif
