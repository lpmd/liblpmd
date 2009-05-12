//
//
//

#ifndef __LPMD_INTEGRATOR_H__
#define __LPMD_INTEGRATOR_H__

#include <lpmd/basiccell.h>
#include <lpmd/basicparticleset.h>

namespace lpmd
{

 class Potential;                    // forward declaration

class Integrator
{
 public:
   Integrator();
   virtual ~Integrator();

   void GoBack(BasicParticleSet & atoms, BasicCell & cell);

   double Timestep() const;

   void UseOldCell(BasicParticleSet & atoms, BasicCell & cell);

   BasicParticleSet & OldAtoms() const;
   BasicCell & OldCell() const;

   virtual void Initialize(BasicParticleSet & atoms, BasicCell & cell, Potential & p);

   virtual void Advance(BasicParticleSet & atoms, BasicCell & cell, Potential & p) = 0;

 protected:
   double dt;

 private:
   BasicParticleSet * oldatoms;
   BasicCell * oldcell;
};

} // lpmd

#endif

