/*
 *
 *
 *
 */

#ifndef __LPMD_CONFIGURATION_SET_H__
#define __LPMD_CONFIGURATION_SET_H__

#include <lpmd/configuration.h>

namespace lpmd
{

 class ConfigurationSet
 {
  public:
    virtual Configuration & operator[](long int i) = 0;

    virtual const Configuration & operator[](long int i) const = 0;

    virtual long int Size() const = 0;
  
    virtual long int Size() = 0;
 };

}  // lpmd

#endif

