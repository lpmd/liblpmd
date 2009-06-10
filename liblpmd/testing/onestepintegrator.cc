//
//
//

#include <lpmd/onestepintegrator.h>
#include <lpmd/simulation.h>
#include <lpmd/potential.h>
#include <lpmd/atom.h>

using namespace lpmd;

void OneStepIntegrator::Advance(Simulation & sim, Potential & p)
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
   else Advance(sim, i);
  }
 }
 else
 {
  for (long int i=0;i<atoms.Size();++i) Advance(sim, i);
 }
}

