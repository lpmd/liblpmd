/*
 *
 *
 *
 */

#ifndef __LPMD_SIMULATION_BUILDER_H__
#define __LPMD_SIMULATION_BUILDER_H__

#include <lpmd/simulation.h>
#include <lpmd/basicparticleset.h>

namespace lpmd
{

 class SimulationBuilder
 {
  public:
    SimulationBuilder();
    ~SimulationBuilder();

    static Simulation & CreateFixedOrthogonal(long int atoms, const BasicAtom & at); 
    static Simulation & CreateGeneric(long int atoms, const BasicAtom & at);
    static Simulation & CreateGeneric();
  
  private:
   static class SimBuildImpl impl;
 };

}  // lpmd

#endif

