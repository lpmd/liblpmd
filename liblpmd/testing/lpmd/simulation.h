//
//
//

#ifndef __LPMD_SIMULATION_H__
#define __LPMD_SIMULATION_H__

#include <lpmd/configuration.h>
#include <lpmd/cell.h>
#include <lpmd/atom.h>
#include <lpmd/array.h>
#include <lpmd/potential.h>
#include <lpmd/neighbor.h>
#include <lpmd/error.h>

// FIXME: no corresponde aqui
const double kboltzmann = 8.6173422E-05;

namespace lpmd
{
 class NoIntegrator: public Error
 {
  public:
    NoIntegrator(): Error("No integrator defined") { }
 };
 class Integrator;
 class CellManager;

 class Simulation: public Configuration
 {
  public:
    virtual ~Simulation() { }

    virtual void SetTemperature(double temp) = 0;

    virtual Array<Potential &> & Potentials() = 0;

    virtual Potential & CombinedPotential() = 0;

    virtual void DoStep() = 0;
 
    virtual void DoSteps(long int n) = 0;

    virtual long int CurrentStep() const = 0;

    virtual void SetIntegrator(lpmd::Integrator & itg) = 0;

    virtual lpmd::Integrator & Integrator() = 0;
 };

 Simulation * FixedOrthogonalEngine(long int atoms, const AtomInterface & at);

 Simulation * GeneralEngine(long int atoms, const AtomInterface & at);

} // lpmd

#endif

