/*
 *
 *
 *
 */

#ifndef __LPMD_BASICCELL_H__
#define __LPMD_BASICCELL_H__

#include <lpmd/vector.h>

namespace lpmd
{

 class BasicCell
 {
  public:
    virtual ~BasicCell() { };

    virtual bool IsOrthogonal() const = 0;

    virtual const bool & Periodicity(int q) const = 0;
    virtual bool & Periodicity(int q) = 0;
    
    virtual Vector & operator[](int q) = 0;
    virtual const Vector & operator[](int q) const = 0;

    virtual Vector ScaleByCell(const BasicVector & cv) const = 0;

    virtual bool IsInside(const BasicVector & v) const = 0;
    virtual Vector FittedInside(const BasicVector & v) const = 0;
    virtual Vector Displacement(const BasicVector & a, const BasicVector & b) const = 0;
    virtual double Volume() const = 0;

 };

} // lpmd

#endif

