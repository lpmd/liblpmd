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
#include "atominterface.h"
#include "elements.h"

namespace lpmd
{

class Atom: public AtomInterface
{
 public: 
  Atom(): AtomInterface(0, &iv[0], &iv[1], &iv[2]), index(0) { }

  Atom(const Atom & at): AtomInterface(at.Z(), &iv[0], &iv[1], &iv[2]), index(0)
  {
   iv[0] = at.Position();
   iv[1] = at.Velocity();
   iv[2] = at.Acceleration();
  }

  Atom(const AtomInterface & at): AtomInterface(at.Z(), &iv[0], &iv[1], &iv[2]), index(0)
  {
   iv[0] = at.Position();
   iv[1] = at.Velocity();
   iv[2] = at.Acceleration();
  }

  Atom(const std::string a): AtomInterface(ElemNum(a), &iv[0], &iv[1], &iv[2]), index(0)
  {
  }

  Atom(const int zeta): AtomInterface(zeta, &iv[0], &iv[1], &iv[2]), index(0)
  {
  }

  Atom(const std::string a, Vector & pos): AtomInterface(ElemNum(a), &iv[0], &iv[1], &iv[2]), index(0)
  {
   iv[0] = pos;
  }

  Atom(const std::string a, Vector & pos, Vector & vel): AtomInterface(ElemNum(a), &iv[0], &iv[1], &iv[2]), index(0)
  {
   iv[0] = pos;
   iv[1] = vel;
  }
 
  Atom(const std::string elem, Vector & pos, Vector & vel, Vector & ace): AtomInterface(ElemNum(elem), &iv[0], &iv[1], &iv[2]), index(0)
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

}

#endif

