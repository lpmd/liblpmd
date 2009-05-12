//
//
//

#ifndef __LPMD_POTENTIAL_H__
#define __LPMD_POTENTIAL_H__

#include <lpmd/basicparticleset.h>
#include <lpmd/basiccell.h>
#include <lpmd/error.h>

namespace lpmd
{

class Potential
{
 public:
   Potential();
   virtual ~Potential();

   void SetValidSpecies(int s1, int s2);

   bool AppliesTo(int s1, int s2) const;

   void SetCutoff(double rc) { rcutoff = rc; }

   double GetCutoff() const { return rcutoff; }

   virtual void Initialize(BasicParticleSet & atoms, BasicCell & cell);

   virtual double energy(BasicParticleSet & atoms, BasicCell & cell) = 0;

   virtual void UpdateForces(BasicParticleSet & atoms, BasicCell & cell) = 0;

 private:
   int spc_sum;
   double rcutoff;

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


