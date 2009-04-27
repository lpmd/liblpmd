/*
 *
 *
 *
 */

#ifndef __LPMD_ATOMINTERFACE_H__
#define __LPMD_ATOMINTERFACE_H__

#include "vector.h"
#include "elements.h"

class AtomInterface
{
 public:
  AtomInterface(int z0): z(z0) { }
  virtual ~AtomInterface() { }

  inline int Z() const { return z; }
  inline std::string Symbol() const { return ElemSym[Z()]; }
  inline Vector & Position() { return iv[0]; }
  inline Vector & Velocity() { return iv[1]; }
  inline Vector & Acceleration() { return iv[2]; }
  inline double Mass() const { return ElemMass[Z()]; }

 protected:
  int z;
  Vector * iv;
};

#endif

