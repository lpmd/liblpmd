/*
 *
 *
 *
 */

#ifndef __LPMD_BASICATOM_H__
#define __LPMD_BASICATOM_H__

#include <lpmd/vector.h>
#include <lpmd/elements.h>

namespace lpmd
{

class BasicAtom
{
 public:
  BasicAtom(int z0, Vector * ip, Vector * iv, Vector * ia): z(z0), ipos(ip), ivel(iv), iacc(ia) { }
  virtual ~BasicAtom() { }

  inline int Z() const { return z; }
  inline std::string Symbol() const { return ElemSym[Z()]; }
  inline const Vector & Position() const { return *ipos; }
  inline Vector & Position() { return *ipos; }
  inline const Vector & Velocity() const { return *ivel; }
  inline Vector & Velocity() { return *ivel; }
  inline const Vector & Acceleration() const { return *iacc; }
  inline Vector & Acceleration() { return *iacc; }
  inline double Mass() const { return ElemMass[Z()]; }

  inline BasicAtom & operator=(const BasicAtom & at) 
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

inline bool operator==(const BasicAtom & a, const BasicAtom & b)
{
 return ((a.Z() == b.Z()) && (a.Position() == b.Position()));
}

}

#endif

