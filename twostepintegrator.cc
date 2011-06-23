//
//
//

#include <lpmd/twostepintegrator.h>
#include <lpmd/simulation.h>
#include <lpmd/potential.h>
#include <lpmd/session.h>

using namespace lpmd;

void TwoStepIntegrator::Advance(Simulation & sim, Potential & p)
{
 p.UpdateForces(sim);
 BasicParticleSet & atoms = sim.Atoms();
 const Vector aczero(0.0, 0.0, 0.0);

 long int i = 0;
 Atom at("H");
 // Setea a cero las aceleraciones de los atomos con fixedvel
 if (atoms.HaveAny(Tag("fixedvel"))) 
 {
  GlobalSession.DebugStream() << "-> Considering the fixedvel flag on some atoms\n";
  for (i=0;i<atoms.Size();++i)
  {
   at = atoms[i];
   if (atoms.Have(at, Tag("fixedvel"))) atoms[i].Acceleration() = aczero;
  }
 }

 if (atoms.HaveAny(Tag("fixedpos")))
 {
  GlobalSession.DebugStream() << "-> Considering the fixedpos flag on some atoms\n";
  for (i=0;i<atoms.Size();++i)
  {
   at = atoms[i];
   if (atoms.Have(at, Tag("fixedpos")) && (atoms.GetTag(at, Tag("fixedpos")) == "true")) continue;
   else AdvancePosition(sim, i);
  }
 }
 else
 {
  for (i=0;i<atoms.Size();++i) AdvancePosition(sim, i);
 }

 p.UpdateForces(sim);

 // Setea a cero las aceleraciones de los atomos con fixedvel
 if (atoms.HaveAny(Tag("fixedvel"))) 
 {
  GlobalSession.DebugStream() << "-> Considering the fixedvel flag on some atoms\n";
  for (i=0;i<atoms.Size();++i)
  {
   at = atoms[i];
   if (atoms.Have(at, Tag("fixedvel"))) atoms[i].Acceleration() = aczero;
  }
 }

 if (atoms.HaveAny(Tag("fixedpos")))
 {
  GlobalSession.DebugStream() << "-> Considering the fixedpos flag on some atoms\n";
  for (i=0;i<atoms.Size();++i)
  {
   at = atoms[i];
   if (atoms.Have(at, Tag("fixedpos")) && (atoms.GetTag(at, Tag("fixedpos")) == "true")) continue;
   else AdvanceVelocity(sim, i);
  }
 }
 else
 {
  for (i=0;i<atoms.Size();++i) AdvanceVelocity(sim, i);
 }

}

