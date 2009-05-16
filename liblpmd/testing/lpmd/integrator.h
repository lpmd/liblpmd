//
//
//

#ifndef __LPMD_INTEGRATOR_H__
#define __LPMD_INTEGRATOR_H__

#include <lpmd/basiccell.h>
#include <lpmd/basicparticleset.h>
#include <lpmd/solver.h>

namespace lpmd
{
 class Simulation; // forward
 class Potential;

class Integrator: public Solver
{
 public:
   Integrator();
   virtual ~Integrator();

   void GoBack(Configuration & conf);

   double Timestep() const;

   void UseOldConfig(Configuration & conf);

   Configuration & OldConfig() const;
   
   void Advance(Simulation & sim) { Advance(sim, sim.CombinedPotential()); }
    
   virtual void Initialize(Simulation & sim, Potential & p);
   virtual void Advance(Simulation & sim, Potential & p) = 0;

 protected:
   double dt;

 private:
   Configuration * oldconfig;
};

} // lpmd

#endif

