/*
 *
 *
 *
 */

#ifndef __LPMD_BASICCELL_H__
#define __LPMD_BASICCELL_H__

#include <lpmd/region.h>

namespace lpmd
{

 class BasicCell: public Region
 {
  public:
    virtual ~BasicCell() { }

    virtual bool IsOrthogonal() const = 0;

    virtual const bool & Periodicity(int q) const = 0;

    virtual bool & Periodicity(int q) = 0;
    
    virtual Vector & operator[](int q) = 0;

    virtual const Vector & operator[](int q) const = 0;

    virtual Vector Fractional(const Vector & v) const = 0;
    
    virtual Vector Cartesian(const Vector & v) const = 0;

    virtual Vector ScaleByCell(const Vector & cv) const = 0;

    virtual Vector FittedInside(const Vector & v) const = 0;

    virtual Vector Displacement(const Vector & a, const Vector & b) const = 0;

 };

} // lpmd

inline bool operator==(const lpmd::BasicCell & cell_a, const lpmd::BasicCell & cell_b)
{
 for (int q=0;q<3;++q)
     if ((cell_a[q]-cell_b[q]).Module() >= 1.0E-08) return false;
 return true;
}

inline bool operator!=(const lpmd::BasicCell & cell_a, const lpmd::BasicCell & cell_b) { return !(operator==(cell_a, cell_b)); }

#endif

