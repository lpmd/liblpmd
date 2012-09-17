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
#include <lpmd/colorhandler.h>

namespace lpmd
{

class Atom: public BasicAtom
{
 public: 
  Atom(): BasicAtom(0, &iv[0], &iv[1], &iv[2]) { }

  Atom(const Atom & at): BasicAtom(at.Z(), &iv[0], &iv[1], &iv[2], at.ID())
  {
   iv[0] = at.Position();
   iv[1] = at.Velocity();
   iv[2] = at.Acceleration();
   //const BasicAtom & basic_at = at;
   //const BasicAtom & my_basicat = (*this);
   //if (ColorHandler::HaveColor(basic_at)) ColorHandler::ColorOfAtom(my_basicat) = ColorHandler::ColorOfAtom(basic_at);
  }

  Atom(const BasicAtom & at): BasicAtom(at.Z(), &iv[0], &iv[1], &iv[2], at.ID())
  {
   iv[0] = at.Position();
   iv[1] = at.Velocity();
   iv[2] = at.Acceleration();
   //const BasicAtom & basic_at = at;
   //const BasicAtom & my_basicat = (*this);
   //if (ColorHandler::HaveColor(basic_at)) ColorHandler::ColorOfAtom(my_basicat) = ColorHandler::ColorOfAtom(basic_at);
  }

  Atom(const std::string a): BasicAtom(ElemNum(a), &iv[0], &iv[1], &iv[2]) { }

  Atom(const int zeta): BasicAtom(zeta, &iv[0], &iv[1], &iv[2]) { }

  Atom(const std::string a, const Vector & pos): BasicAtom(ElemNum(a), &iv[0], &iv[1], &iv[2]) { iv[0] = pos; }
  
  Atom(const int zeta, const Vector & pos): BasicAtom(zeta, &iv[0], &iv[1], &iv[2]) { iv[0] = pos; }

  Atom(const std::string a, const Vector & pos, const Vector & vel): BasicAtom(ElemNum(a), &iv[0], &iv[1], &iv[2])
  {
   iv[0] = pos;
   iv[1] = vel;
  }
 
  Atom(const int zeta, const Vector & pos, const Vector & vel): BasicAtom(zeta, &iv[0], &iv[1], &iv[2])
  {
   iv[0] = pos;
   iv[1] = vel;
  }

  Atom(const std::string elem, const Vector & pos, const Vector & vel, const Vector & ace): BasicAtom(ElemNum(elem), &iv[0], &iv[1], &iv[2])
  {
   iv[0] = pos;
   iv[1] = vel;
   iv[2] = ace;
  }
  
  Atom(const int zeta, const Vector & pos, const Vector & vel, const Vector & ace): BasicAtom(zeta, &iv[0], &iv[1], &iv[2])
  {
   iv[0] = pos;
   iv[1] = vel;
   iv[2] = ace;
  }

  ~Atom() { } 

 private:
  Vector iv[3];
};

inline bool operator==(const Atom & a, const Atom & b)
{
 return ((a.Z() == b.Z()) && (a.Position() == b.Position()));
}

}  // lpmd

#endif

