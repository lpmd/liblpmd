//
//
//

#ifndef __LPMD_SIMULATION_H__
#define __LPMD_SIMULATION_H__

#include <lpmd/filtrableconfiguration.h>
#include <lpmd/cell.h>
#include <lpmd/atom.h>
#include <lpmd/array.h>
#include <lpmd/potential.h>
#include <lpmd/atompair.h>

// FIXME: no corresponde aqui
const double kboltzmann = 8.6173422E-05;

namespace lpmd
{
 class Integrator;  // forward
 class CombinedPotential; // forward

 class Simulation: public FiltrableConfiguration
 {
  public:
    virtual ~Simulation() { }

    virtual void SetTemperature(double temp) = 0;

    virtual CombinedPotential & Potentials() = 0;

    virtual void DoStep() = 0;
 
    virtual void DoSteps(long int n) = 0;

    virtual long int CurrentStep() const = 0;

    virtual void SetIntegrator(lpmd::Integrator & itg) = 0;

    virtual lpmd::Integrator & Integrator() = 0;
 };

} // lpmd

#endif

