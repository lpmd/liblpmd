/*
 *
 *
 *
 */

#ifndef __LPMD_ATOMINTERFACE_H__
#define __LPMD_ATOMINTERFACE_H__

#include <lpmd/basicvector.h>
#include "elements.h"

namespace lpmd
{

class AtomInterface
{
 public:
  AtomInterface(int z0): z(z0) { }
  virtual ~AtomInterface() { }

  inline int Z() const { return z; }
  inline std::string Symbol() const { return ElemSym[Z()]; }
  inline const BasicVector & Position() const { return iv[0]; }
  inline BasicVector & Position() { return iv[0]; }
  inline const BasicVector & Velocity() const { return iv[1]; }
  inline BasicVector & Velocity() { return iv[1]; }
  inline const BasicVector & Acceleration() const { return iv[2]; }
  inline BasicVector & Acceleration() { return iv[2]; }
  inline double Mass() const { return ElemMass[Z()]; }

  inline AtomInterface & operator=(const AtomInterface & at) 
  {
   if (&at != this)
   {
    z = at.Z();
    Position() = at.Position(); 
    Velocity() = at.Velocity(); 
    Acceleration() = at.Acceleration(); 
   }
   return (*this);
  }

 protected:
  int z;
  BasicVector * iv;
};

inline bool operator==(const AtomInterface & a, const AtomInterface & b)
{
 return ((a.Z() == b.Z()) && ((a.Position()-b.Position()).Module() < 1.0E-10));
}

}

#endif

