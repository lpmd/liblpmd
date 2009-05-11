/*
 *
 *
 *
 */

#ifndef __LPMD_BASICPARTICLESET_H__
#define __LPMD_BASICPARTICLESET_H__

namespace lpmd
{

class BasicParticleSet
{
 public:

  virtual Atom & operator[](long int i) = 0;

  virtual const Atom & operator[](long int i) const = 0;

  virtual long int Size() const = 0;

  virtual void Append(const Atom & x) = 0;

  virtual void Clear() = 0;

  virtual void Delete(long int i) = 0;

  virtual long int Find(const Atom & t) = 0;
};

} // lpmd

#endif

