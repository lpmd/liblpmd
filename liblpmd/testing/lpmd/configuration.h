/*
 *
 *
 *
 */

#ifndef __LPMD_CONFIGURATION_H__
#define __LPMD_CONFIGURATION_H__

#include <lpmd/basicparticleset.h>
#include <lpmd/basiccell.h>

namespace lpmd
{
 class Configuration
 {
  public:
    virtual ~Configuration() { };
    virtual BasicParticleSet & Atoms() = 0; 
    virtual const BasicParticleSet & Atoms() const = 0;
    virtual BasicCell & Cell() = 0;
    virtual const BasicCell & Cell() const = 0;
 };
}  // lpmd

#endif

