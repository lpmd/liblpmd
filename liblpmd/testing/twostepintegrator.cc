//
//
//

#include <lpmd/twostepintegrator.h>
#include <lpmd/simulation.h>
#include <lpmd/potential.h>

using namespace lpmd;

void TwoStepIntegrator::Advance(Simulation & sim, Potential & p)
{
 p.UpdateForces(sim);
 BasicParticleSet & atoms = sim.Atoms();
 const Vector aczero(0.0, 0.0, 0.0);
 
 // Setea a cero las aceleraciones de los atomos con fixedvel
 if (atoms.HaveAny(Tag("fixedvel"))) 
 {
  for (long int i=0;i<atoms.Size();++i)
  {
   const Atom & at = atoms[i];
   if (atoms.Have(at, Tag("fixedvel"))) atoms[i].Acceleration() = aczero;
  }
 }

 if (atoms.HaveAny(Tag("fixedpos")))
 {
  for (long int i=0;i<atoms.Size();++i)
  {
   const Atom & at = atoms[i];
   if (atoms.Have(at, Tag("fixedpos"))) continue;
   else AdvancePosition(sim, i);
  }
 }
 else
 {
  for (long int i=0;i<atoms.Size();++i) AdvancePosition(sim, i);
 }

 p.UpdateForces(sim);

 // Setea a cero las aceleraciones de los atomos con fixedvel
 if (atoms.HaveAny(Tag("fixedvel"))) 
 {
  for (long int i=0;i<atoms.Size();++i)
  {
   const Atom & at = atoms[i];
   if (atoms.Have(at, Tag("fixedvel"))) atoms[i].Acceleration() = aczero;
  }
 }

 if (atoms.HaveAny(Tag("fixedpos")))
 {
  for (long int i=0;i<atoms.Size();++i)
  {
   const Atom & at = atoms[i];
   if (atoms.Have(at, Tag("fixedpos"))) continue;
   else AdvanceVelocity(sim, i);
  }
 }
 else
 {
  for (long int i=0;i<atoms.Size();++i) AdvanceVelocity(sim, i);
 }

}

