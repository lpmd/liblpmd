/*
 *
 *
 *
 */

#ifndef __LPMD_SIMULATION_ENGINE_H__
#define __LPMD_SIMULATION_ENGINE_H__

#include <lpmd/simulation.h>
#include <lpmd/properties.h>
#include <lpmd/particleset.h>
#include <lpmd/fixedsizeparticleset.h>
#include <lpmd/orthogonalcell.h>
#include <lpmd/nonorthogonalcell.h>
#include <lpmd/combinedpotential.h>
#include <lpmd/integrator.h>
#include <lpmd/cellmanager.h>

namespace lpmd
{

 class SimulationBuilder
 {
  public:
    SimulationBuilder();
    ~SimulationBuilder();
    Simulation & CreateFixedOrthogonal(long int atoms, const BasicAtom & at); 
    Simulation & CreateGeneric(long int atoms, const BasicAtom & at);
  
  private:
   class SimBuildImpl * impl;  // pointer to implementation 
 };

}  // lpmd

#endif

