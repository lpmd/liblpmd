/*
 *
 *
 *
 */

#ifndef __LPMD_ATOM_H__
#define __LPMD_ATOM_H__

#include <cstdlib>
#include <string>
#include <lpmd/vector.h>
#include <lpmd/basicatom.h>
#include <lpmd/elements.h>

namespace lpmd
{

class Atom: public BasicAtom
{
 public: 
  Atom(): BasicAtom(0, &iv[0], &iv[1], &iv[2]), index(0) { }

  Atom(const Atom & at): BasicAtom(at.Z(), &iv[0], &iv[1], &iv[2]), index(0)
  {
   iv[0] = at.Position();
   iv[1] = at.Velocity();
   iv[2] = at.Acceleration();
  }

  Atom(const BasicAtom & at): BasicAtom(at.Z(), &iv[0], &iv[1], &iv[2]), index(0)
  {
   iv[0] = at.Position();
   iv[1] = at.Velocity();
   iv[2] = at.Acceleration();
  }

  Atom(const std::string a): BasicAtom(ElemNum(a), &iv[0], &iv[1], &iv[2]), index(0) { }

  Atom(const int zeta): BasicAtom(zeta, &iv[0], &iv[1], &iv[2]), index(0) { }

  Atom(const std::string a, Vector & pos): BasicAtom(ElemNum(a), &iv[0], &iv[1], &iv[2]), index(0) { iv[0] = pos; }

  Atom(const std::string a, Vector & pos, Vector & vel): BasicAtom(ElemNum(a), &iv[0], &iv[1], &iv[2]), index(0)
  {
   iv[0] = pos;
   iv[1] = vel;
  }
 
  Atom(const std::string elem, Vector & pos, Vector & vel, Vector & ace): BasicAtom(ElemNum(elem), &iv[0], &iv[1], &iv[2]), index(0)
  {
   iv[0] = pos;
   iv[1] = vel;
   iv[2] = ace;
  }

  ~Atom() { } 

 private:
  long index; //index atom
  Vector iv[3];
};

inline bool operator==(const Atom & a, const Atom & b)
{
 return ((a.Z() == b.Z()) && (a.Position() == b.Position()));
}

}  // lpmd

#endif

