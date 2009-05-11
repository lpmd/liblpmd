/*
 *
 *
 *
 */

#ifndef __LPMD_BASICPARTICLESET_H__
#define __LPMD_BASICPARTICLESET_H__

#include <lpmd/atominterface.h>

namespace lpmd
{

class AtomArray
{
 public:
  virtual ~AtomArray() { };

  virtual AtomInterface & operator[](long int i) = 0;

  virtual const AtomInterface & operator[](long int i) const = 0;

  virtual long int Size() const = 0;

  virtual long int Find(const AtomInterface & t) = 0;
};

class MutableAtomArray: public AtomArray
{
 public:
  virtual ~MutableAtomArray() { };

  virtual void Append(const AtomInterface & x) = 0;

  virtual void Clear() = 0;

  virtual void Delete(long int i) = 0;

};

typedef MutableAtomArray BasicParticleSet;

} // lpmd

#endif

