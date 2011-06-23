/*
 *
 *
 *
 */

#ifndef __LPMD_PARAMETER_H__
#define __LPMD_PARAMETER_H__

#include <string>

namespace lpmd
{

 class Parameter: public std::string
 {
  public:
   Parameter(): std::string() { }
   Parameter(const std::string & p): std::string(p) { }

   Parameter & operator=(const std::string & p)
   {
    if (&p != this) { std::string::operator=(p); }
    return (*this);
   }

   inline operator double() { return double(atof((*this).c_str())); }
   inline operator bool() { return bool((*this) == "true"); }
   inline operator int() { return atoi((*this).c_str()); }
 };

}  // lpmd


#endif

