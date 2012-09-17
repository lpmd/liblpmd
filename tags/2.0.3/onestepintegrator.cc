//
//
//

#include <lpmd/onestepintegrator.h>
#include <lpmd/simulation.h>
#include <lpmd/potential.h>
#include <lpmd/session.h>
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
  GlobalSession.DebugStream() << "-> Considering the fixedvel flag on some atoms\n";
  for (long int i=0;i<atoms.Size();++i)
  {
   const BasicAtom & at = atoms[i];
   if (atoms.Have(at, Tag("fixedvel"))) atoms[i].Acceleration() = aczero;
  }
 }

 if (atoms.HaveAny(Tag("fixedpos")))
 {
  GlobalSession.DebugStream() << "-> Considering the fixedpos flag on some atoms\n";
  for (long int i=0;i<atoms.Size();++i)
  {
   const BasicAtom & at = atoms[i];
   if (atoms.Have(at, Tag("fixedpos")) && (atoms.GetTag(at, Tag("fixedpos")) == "true")) continue;
   else Advance(sim, i);
  }
 }
 else
 {
  for (long int i=0;i<atoms.Size();++i) Advance(sim, i);
 }
}

