//
//
//

#ifndef __LPMD_COLOR_H__
#define __LPMD_COLOR_H__

#include <lpmd/vector.h>

namespace lpmd
{

 class Color: public Vector
 {
  public:
    Color(): Vector(0.0, 0.0, 0.0) { }
    Color(double red, double green, double blue): Vector(red, green, blue) { }
    Color(const Vector & rgb): Vector(rgb) { }
    Color(const Color & color): Vector(color) { }

    inline double & Red() { return (*this)[0]; }
    inline double & Green() { return (*this)[1]; }
    inline double & Blue() { return (*this)[2]; }

    inline const double & Red() const { return (*this)[0]; }
    inline const double & Green() const { return (*this)[1]; }
    inline const double & Blue() const { return (*this)[2]; }
 };


} // lpmd

#endif
