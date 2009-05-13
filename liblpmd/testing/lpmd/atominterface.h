/*
 *
 *
 *
 */

#ifndef __LPMD_ATOMINTERFACE_H__
#define __LPMD_ATOMINTERFACE_H__

#include <lpmd/vector.h>
#include "elements.h"

namespace lpmd
{

class AtomInterface
{
 public:
  AtomInterface(int z0, Vector * ip, Vector * iv, Vector * ia): z(z0), ipos(ip), ivel(iv), iacc(ia) { }
  virtual ~AtomInterface() { }

  inline int Z() const { return z; }
  inline std::string Symbol() const { return ElemSym[Z()]; }
  inline const Vector & Position() const { return *ipos; }
  inline Vector & Position() { return *ipos; }
  inline const Vector & Velocity() const { return *ivel; }
  inline Vector & Velocity() { return *ivel; }
  inline const Vector & Acceleration() const { return *iacc; }
  inline Vector & Acceleration() { return *iacc; }
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
  Vector * ipos, * ivel, * iacc;
};

inline bool operator==(const AtomInterface & a, const AtomInterface & b)
{
 return ((a.Z() == b.Z()) && (a.Position() == b.Position()));
}

}

#endif

