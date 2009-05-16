/*
 *
 *
 *
 */

#ifndef __LPMD_STORED_CONFIGURATION_H__
#define __LPMD_STORED_CONFIGURATION_H__

#include <lpmd/configuration.h>
#include <lpmd/particleset.h>
#include <lpmd/cell.h>

namespace lpmd
{
 // FIXME: esto huele a FactoryMethod...
 class StoredConfiguration: public Configuration
 {
  public:
    StoredConfiguration() { }

    StoredConfiguration(const Configuration & conf)
    {
     atoms = conf.Atoms();
     cell = conf.Cell();
    }

    inline BasicParticleSet & Atoms() { return atoms; }
    inline const BasicParticleSet & Atoms() const { return atoms; }
    BasicCell & Cell() { return cell; }
    inline const BasicCell & Cell() const { return cell; }
 
  private:
    ParticleSet atoms;
    NonOrthogonalCell cell;
 };
}  // lpmd

#endif

