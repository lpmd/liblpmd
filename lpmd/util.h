//
//
//
#ifndef __LPMD_UTIL_H__
#define __LPMD_UTIL_H__

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>

#include <lpmd/vector.h>

namespace lpmd
{

template <class T> const std::string ToString(const T & object)
{
 std::ostringstream os;
 os << std::boolalpha << object;
 return(os.str());
}

}

#endif

