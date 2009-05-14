//
//
//

#ifndef __LPMD_POTENTIAL_H__
#define __LPMD_POTENTIAL_H__

#include <lpmd/error.h>
#include <lpmd/basiccell.h>
#include <lpmd/basicparticleset.h>

namespace lpmd
{
 class Simulation;   // forward

class Potential
{
 public:
   Potential();
   virtual ~Potential();

   void SetValidSpecies(int s1, int s2);

   bool AppliesTo(int s1, int s2) const;

   void SetCutoff(double rc) { rcutoff = rc; }

   double GetCutoff() const { return rcutoff; }

   Simulation & GetSimulation() { assert(innersim != 0); return *innersim; }

   virtual void Initialize(Simulation & sim);

   virtual double energy(BasicParticleSet & atoms, BasicCell & cell) = 0;
   virtual void UpdateForces(BasicParticleSet & atoms, BasicCell & cell) = 0;

   double energy(Simulation & sim);
   void UpdateForces(Simulation & sim);

 private:
   int spc_sum;
   double rcutoff;
   Simulation * innersim;

 protected:
   double energycache;
};

 class HorrendousForce: public Error
 {
  public:
    HorrendousForce(double mag);
 };

} // lpmd

#endif


