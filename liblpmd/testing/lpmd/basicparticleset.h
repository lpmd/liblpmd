/*
 *
 *
 *
 */

#ifndef __LPMD_BASICPARTICLESET_H__
#define __LPMD_BASICPARTICLESET_H__

#include <lpmd/basicatom.h>
#include <lpmd/atompair.h>
#include <lpmd/array.h>
#include <lpmd/taghandler.h>

namespace lpmd
{

class AtomArray: public TagHandler<BasicAtom>
{
 public:
  virtual ~AtomArray() { };

  virtual BasicAtom & operator[](long int i) = 0;

  virtual const BasicAtom & operator[](long int i) const = 0;

  virtual long int Size() const = 0;

  virtual long int Find(const BasicAtom & t) = 0;

  virtual const Array<int> & Elements() const = 0;
};

class MutableAtomArray: public AtomArray
{
 public:
  virtual ~MutableAtomArray() { };

  virtual void Append(const BasicAtom & x) = 0;

  virtual void Clear() = 0;

  virtual void Delete(long int i) = 0;

};

typedef MutableAtomArray BasicParticleSet;

} // lpmd

#endif

